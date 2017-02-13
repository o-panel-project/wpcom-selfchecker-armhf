/*
 * TCTL INI read
 * $Id: tctlprof.c 110 2012-03-16 01:32:41Z k-morita $
 */

#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/stat.h>

/* Define */
#define BUF_SIZE				256
#define ALLOC_SIZE				10

#ifndef BOOL
#define	BOOL int
#endif

/* Global Variables */
typedef struct _KEY {
	char	key_name[BUF_SIZE];
	int		hash;
	char	*string;
	BOOL	comment_flag;
} KEY_INFO;

typedef struct _SECTION {
	char	section_name[BUF_SIZE];
	int		hash;
	KEY_INFO	*key_info;
	int		key_count;
	int		key_size;
} SECTION_INFO;

static SECTION_INFO *section_info;
static int section_count;
static int section_size;

/*
 * str_cpy_n - 文字列のコピー
 */
static void str_cpy_n(char *dst, const char *src, int len)
{
	while (--len && (*(dst++) = *(src++)));
	*dst = 0;
}

/*
 * trim - 文字列の前後の空白, Tabを除去する
 */
static BOOL trim(char *buf)
{
	char *p, *r;

	/* 前後の空白を除いたポインタを取得 */
	for (p = buf; (*p == ' ' || *p == '\t') && *p != '\0'; p++) ;
	for (r = buf + strlen(buf) - 1; r > p && (*r == ' ' || *r == '\t'); r--) ;
	*(r + 1) = '\0';

	/* 元の文字列にコピー */
	strcpy(buf, p);
	return 1;
}

/*
 * str2hash - 文字列のハッシュ値を取得
 */
static int str2hash(const char *str)
{
#define to_lower(c)		((c >= 'A' && c <= 'Z') ? (c - 'A' + 'a') : c)
	int hash = 0;

	for (; *str != '\0'; str++) {
		if (*str != ' ') {
			hash = ((hash << 1) + to_lower(*str));
		}
	}
	return hash;
}

/*
 * check_file - ファイルが存在するかチェック
 */
static BOOL check_file(const char *fname)
{
	if (access(fname, R_OK) == 0)
		return 1;
	else
		return 0;
}

/*
 * メモリ割り当て関連
 */
static void *mem_alloc(const unsigned long size)
{
	return malloc(size);
}

static void *mem_calloc(const unsigned long size)
{
	return calloc(1, size);
}

static void mem_free(void **mem)
{
	if (*mem != NULL) {
		free(*mem);
		*mem = NULL;
	}
}

/*
 * section_add - セクションの追加
 */
static BOOL section_add(const char *section_name)
{
	SECTION_INFO *tmp_section;

	if (section_name == NULL || *section_name == '\0') {
		return 0;
	}

	if (section_size < section_count + 1) {
		/* 再確保 */
		section_size += ALLOC_SIZE;
		if ((tmp_section = (SECTION_INFO *)mem_calloc(sizeof(SECTION_INFO) * section_size)) == NULL) {
			return 0;
		}
		if (section_info != NULL) {
			memcpy(tmp_section, section_info, sizeof(SECTION_INFO) * section_count);
			mem_free((void **)&section_info);
		}
		section_info = tmp_section;
	}
	/* セクション追加 */
	str_cpy_n((section_info + section_count)->section_name, section_name, BUF_SIZE);
	trim((section_info + section_count)->section_name);
	(section_info + section_count)->hash = str2hash((section_info + section_count)->section_name);

	section_count++;
	return 1;
}

/*
 * section_find - セクションの検索
 */
static int section_find(const char *section_name)
{
	int hash;
	int i;

	if (section_info == NULL ||
		section_name == NULL ||
		*section_name == '\0') {
		return -1;
	}

	hash = str2hash(section_name);
	for (i = 0; i < section_count; i++) {
		if ((section_info + i)->hash != hash) {
			continue;
		}
		if (strcasecmp((section_info + i)->section_name, section_name) == 0) {
			return i;
		}
	}
	return -1;
}

/*
 * key_add - キーの追加
 */
static BOOL key_add(
	SECTION_INFO *si,
	const char *key_name,
	const char *str,
	const BOOL comment_flag)
{
	KEY_INFO *tmp_key;

	if (key_name == NULL || *key_name == '\0' || str == NULL) {
		return 0;
	}

	if (si->key_size < si->key_count + 1) {
		/* 再確保 */
		si->key_size += ALLOC_SIZE;
		if ((tmp_key = (KEY_INFO *)mem_calloc(sizeof(KEY_INFO) * si->key_size)) == NULL) {
			return 0;
		}
		if (si->key_info != NULL) {
			memcpy(tmp_key, si->key_info, sizeof(KEY_INFO) * si->key_count);
			mem_free((void **)&si->key_info);
		}
		si->key_info = tmp_key;
	}
	/* キー追加 */
	(si->key_info + si->key_count)->string = (char *)mem_alloc(sizeof(char) * (strlen(str) + 1));
	if ((si->key_info + si->key_count)->string == NULL) {
		return 0;
	}
	str_cpy_n((si->key_info + si->key_count)->key_name, key_name, BUF_SIZE);
	trim((si->key_info + si->key_count)->key_name);
	if (comment_flag == 0) {
		(si->key_info + si->key_count)->hash = str2hash((si->key_info + si->key_count)->key_name);
	}
	strcpy((si->key_info + si->key_count)->string, str);
	(si->key_info + si->key_count)->comment_flag = comment_flag;

	si->key_count++;
	return 1;
}

/*
 * key_find - キーの検索
 */
static int key_find(const SECTION_INFO *si, const char *key_name)
{
	int hash;
	int i;

	if (si->key_info == NULL || key_name == NULL || *key_name == '\0') {
		return -1;
	}

	hash = str2hash(key_name);
	for (i = 0; i < si->key_count; i++) {
		if ((si->key_info + i)->comment_flag == 1 ||
			(si->key_info + i)->hash != hash) {
			continue;
		}
		if (strcasecmp((si->key_info + i)->key_name, key_name) == 0) {
			return i;
		}
	}
	return -1;
}

/*
 * profile_initialize - 初期化
 */
int comn_ProfileSetup(const char *file_path)
{
	int fd;
	struct stat st;
	long file_size;
	char *cbuf, *buf, *p, *r, *s;
	char tmp[BUF_SIZE];

	if (!check_file(file_path)) {
		section_info = NULL;
		return 1;
	}
	if ((fd = open(file_path, O_RDONLY)) == -1) {
		section_info = NULL;
		return 1;
	}
	if (fstat(fd, &st) == 0)
		file_size = st.st_size;
	else {
		section_info = NULL;
		return 1;
	}

	/* 読み取る領域の確保 */
	if ((cbuf = (char *)mem_alloc(file_size + 1)) == NULL) {
		close(fd);
		section_info = NULL;
		return 1;
	}
	/* ファイルを読み込む */
	if (read(fd, cbuf, file_size) != file_size) {
		mem_free((void **)&cbuf);
		close(fd);
		section_info = NULL;
		return 1;
	}
	close(fd);
	*(cbuf + file_size) = '\0';
	buf = cbuf;

	/* セクションの確保 */
	if ((section_info = (SECTION_INFO *)mem_calloc(sizeof(SECTION_INFO) * ALLOC_SIZE)) == NULL) {
		mem_free((void **)&buf);
		return 1;
	}
	section_count = 1;
	section_size = ALLOC_SIZE;

	/* UTF8のBOMスキップ */
	if ((3 <= file_size) && ((unsigned char)(buf[0]) == 0xEF) && ((unsigned char)(buf[1]) == 0xBB) && ((unsigned char)(buf[2]) == 0xBF)) {
		printf("SKIP UTF8 BOM\n");
		p = buf + 3;
	} else {
		p = buf;
	}

	while ((file_size > (p - buf)) && *p != '\0') {
		for (r = p; (file_size > (r - buf)) && (*r != '\r' && *r != '\n'); r++)
			;

		switch (*p) {
		case '[':
			/* セクションの追加 */
			if (p == r || *(r - 1) != ']') {
				break;
			}
			*(r - 1) = '\0';
			section_add(p + 1);
			break;

		case '\r':
		case '\n':
			break;

		default:
			if (section_info == NULL || p == r) {
				break;
			}
			if (*p == '#') {
				/* コメント */
				for (s = tmp; p < r; p++, s++) {
					*s = *p;
				}
				*s = '\0';
				key_add((section_info + section_count - 1), tmp, "", 1);
			} else {
				/* キーの追加 */
				for (s = tmp; p < r; p++, s++) {
					if (*p == '=') {
						break;
					}
					*s = *p;
				}
				*s = '\0';
				if (*p == '=') {
					p++;
				}
				*r = '\0';
				key_add((section_info + section_count - 1), tmp, p, 0);
			}
			if (file_size > (r - buf)) {
				r++;
			}
		}
		p = r;
		for (; (file_size > (p - buf)) && (*p == '\r' || *p == '\n'); p++)
			;
	}
	mem_free((void **)&buf);
	return 1;
}

/*
 * profile_free - 設定情報の解放
 */
int comn_ProfileTerm(void)
{
	int i, j;

	if (section_info == NULL) {
		return 0;
	}
	for (i = 0; i < section_count; i++) {
		if ((section_info + i)->key_info == NULL) {
			continue;
		}
		/* キーの解放 */
		for (j = 0; j < (section_info + i)->key_count; j++) {
			if (((section_info + i)->key_info + j)->string != NULL) {
				mem_free((void **)&((section_info + i)->key_info + j)->string);
			}
		}
		mem_free((void **)&(section_info + i)->key_info);
	}
	mem_free((void **)&section_info);
	section_info = NULL;
	section_count = 0;
	section_size = 0;

	return 1;
}

/*
 * profile_get_string - 文字列の取得
 */
int comn_GetProfileString(
	const char *section_name,
	const char *key_name,
	const char *default_str,
	char *ret,
	const long size,
	const char *file_path)
{
	char *buf, *p;
	int section_index;
	int key_index;
	int len;

	/* セクションの検索 */
	if ((section_index = section_find(section_name)) == -1) {
		if (default_str)
			str_cpy_n(ret, default_str, size);
		else
			*ret = '\0';
		return strlen(ret);
	}

	/* キーの検索 */
	key_index = key_find((section_info + section_index), key_name);
	if (key_index == -1 || ((section_info + section_index)->key_info + key_index)->string == NULL) {
		if (default_str)
			str_cpy_n(ret, default_str, size);
		else
			*ret = '\0';
		return strlen(ret);
	}

	/* 内容の取得 */
	buf = (char *)mem_alloc(sizeof(char) * (strlen(((section_info + section_index)->key_info + key_index)->string) + 1));
	if (buf != NULL) {
		strcpy(buf, ((section_info + section_index)->key_info + key_index)->string);
		trim(buf);
		p = (*buf == '\"') ? buf + 1 : buf;
		if ((len = strlen(p)) > 0 && *(p + len - 1) == '\"') {
			*(p + len - 1) = '\0';
		}
		str_cpy_n(ret, p, size);
		mem_free((void **)&buf);
	} else {
		str_cpy_n(ret, ((section_info + section_index)->key_info + key_index)->string, size);
	}
	return strlen(ret);
}

/*
 * profile_get_int - 数値の取得
 */
int comn_GetProfileInt(
	const char *section_name,
	const char *key_name,
	const int default_str,
	const char *file_path)
{
	char *buf, *p;
	int section_index;
	int key_index;
	int ret;
	int len;

	/* セクションの検索 */
	if ((section_index = section_find(section_name)) == -1) {
		return default_str;
	}

	/* キーの検索 */
	key_index = key_find((section_info + section_index), key_name);
	if (key_index == -1 || ((section_info + section_index)->key_info + key_index)->string == NULL) {
		return default_str;
	}

	/* 内容の取得 */
	buf = (char *)mem_alloc(sizeof(char) * (strlen(((section_info + section_index)->key_info + key_index)->string) + 1));
	if (buf != NULL) {
		strcpy(buf, ((section_info + section_index)->key_info + key_index)->string);
		trim(buf);
		p = (*buf == '\"') ? buf + 1 : buf;
		if ((len = strlen(p)) > 0 && *(p + len - 1) == '\"') {
			*(p + len - 1) = '\0';
		}
		ret = atoi(p);
		mem_free((void **)&buf);
	} else {
		ret = atoi(((section_info + section_index)->key_info + key_index)->string);
	}
	return ret;
}

/*
 * profile_flush - バッファをファイルに書き込む
 */
int comn_ProfileFlush(const char *file_path)
{
	int fd;
	char *buf, *p, *name;
	int len;
	int i, j;

	if (section_info == NULL) {
		return 0;
	}

	// 保存サイズの計算
	len = 0;
	for (i = 0; i < section_count; i++) {
		if ((section_info + i)->key_info == NULL) {
			continue;
		}
		name = (section_info + i)->section_name;
		// セクション名
		if (i != 0) {
			len += strlen(name) + 4;
		}
		for (j = 0; j < (section_info + i)->key_count; j++) {
			if (*((section_info + i)->key_info + j)->key_name == '\0') {
				continue;
			}
			// キー名
			len += strlen(((section_info + i)->key_info + j)->key_name);
			if (((section_info + i)->key_info + j)->comment_flag == 0) {
				len++;
				if (((section_info + i)->key_info + j)->string != NULL) {
					// 文字列
					len += strlen(((section_info + i)->key_info + j)->string);
				}
			}
			len += 2;
		}
		len += 2;
	}

	// 保存するための領域の確保
	if ((p = buf = (char *)mem_alloc(sizeof(char) * (len + 1))) == NULL) {
		return 0;
	}
	// 保存文字列の作成
	for (i = 0; i < section_count; i++) {
		if ((section_info + i)->key_info == NULL) {
			continue;
		}
		name = (section_info + i)->section_name;
		// セクション名
		if (i != 0) {
			*(p++) = '[';
			strcpy(p, name);
			p += strlen(p);
			*(p++) = ']';
			*(p++) = '\r';
			*(p++) = '\n';
		}
		for (j = 0; j < (section_info + i)->key_count; j++) {
			if (*((section_info + i)->key_info + j)->key_name == '\0') {
				continue;
			}
			// キー名
			strcpy(p, ((section_info + i)->key_info + j)->key_name);
			p += strlen(p);
			if (((section_info + i)->key_info + j)->comment_flag == 0) {
				*(p++) = '=';
				if (((section_info + i)->key_info + j)->string != NULL) {
					// 文字列
					strcpy(p, ((section_info + i)->key_info + j)->string);
					p += strlen(p);
				}
			}
			*(p++) = '\r';
			*(p++) = '\n';
		}
		*(p++) = '\r';
		*(p++) = '\n';
	}
	*p = '\0';

	if ((fd = open(file_path, O_WRONLY|O_CREAT|O_TRUNC, 0644)) < 0) {
		mem_free((void **)&buf);
		return 0;
	}
	if (write(fd, buf, len) != 0) {
		mem_free((void **)&buf);
		close(fd);
		return 0;
	}
	mem_free((void **)&buf);
	fsync(fd);
	close(fd);
	return 1;
}

/*
 * profile_write_data - データの書き込み
 */
static int profile_write_data(
		const char *section_name, const char *key_name, const char *str)
{
	int section_index;
	int key_index;
	int j;

	if (section_name == NULL) {
		return 0;
	}

	if (section_info == NULL) {
		// セクションの確保
		if ((section_info = (SECTION_INFO *)mem_calloc(sizeof(SECTION_INFO) * ALLOC_SIZE)) == NULL) {
			return 0;
		}
		section_count = 1;
		section_size = ALLOC_SIZE;
	}

	// セクションの検索
	if ((section_index = section_find(section_name)) == -1) {
		// セクションの追加
		if (section_add(section_name) == 0) {
			return 0;
		}
		section_index = section_count - 1;
	}

	if (key_name == NULL) {
		if ((section_info + section_index)->key_info != NULL) {
			// キーの削除
			for (j = 0; j < (section_info + section_index)->key_count; j++) {
				if (((section_info + section_index)->key_info + j)->string != NULL) {
					mem_free((void **)&((section_info + section_index)->key_info + j)->string);
				}
			}
			mem_free((void **)&(section_info + section_index)->key_info);
			(section_info + section_index)->key_info = NULL;
			(section_info + section_index)->key_count = 0;
			(section_info + section_index)->key_size = 0;
		}
		return 1;
	}

	// キーの検索
	if ((key_index = key_find((section_info + section_index), key_name)) == -1) {
		// キーの追加
		return key_add((section_info + section_index), key_name, str, 0);
	} else {
		// 内容の変更
		if (((section_info + section_index)->key_info + key_index)->string != NULL) {
			mem_free((void **)&((section_info + section_index)->key_info + key_index)->string);
		}
		if (str == NULL) {
			*((section_info + section_index)->key_info + key_index)->key_name = '\0';
			((section_info + section_index)->key_info + key_index)->string = NULL;
			return 1;
		}
		((section_info + section_index)->key_info + key_index)->string = (char *)mem_alloc(sizeof(char) * (strlen(str) + 1));
		if (((section_info + section_index)->key_info + key_index)->string == NULL) {
			return 0;
		}
		strcpy(((section_info + section_index)->key_info + key_index)->string, str);
	}
	return 1;
}

/*
 * profile_write_string - 文字列の書き込み
 */
int comn_PutProfileString(
		const char *section_name, const char *key_name, const char *str)
{
	char *buf, *p;
	int ret;

	if (str == NULL || *str == '\0') {
		return profile_write_data(section_name, key_name, "");
	}

	if ((buf = (char *)mem_alloc(sizeof(char) * (strlen(str) + 3))) == NULL) {
		return profile_write_data(section_name, key_name, str);
	}
	p = buf;
//	*(p++) = '"';
	strcpy(p, str);
	p += strlen(p);
//	*(p++) = '"';
	*(p++) = '\0';
	ret = profile_write_data(section_name, key_name, buf);
	mem_free((void **)&buf);
	return ret;
}

/*
 * profile_write_int - 数値の書き込み
 */
int comn_PutProfileInt(
		const char *section_name, const char *key_name, const int num)
{
	char ret[BUF_SIZE];

	sprintf(ret, "%d", num);
	return profile_write_data(section_name, key_name, ret);
}

/*
 * profile_delete_key - delete specific key from profile section (GJC)
 */
int comn_DelProfileKey(const char *section_name, const char *key_name)
{
	int section_index;
	int key_index;

	if ((section_index = section_find(section_name)) == -1) {
		return 0;
	}
	if ((key_index = key_find((section_info + section_index), key_name)) == -1) {
		return 0;
	}
	if (((section_info + section_index)->key_info + key_index)->string != NULL) {
		mem_free((void **)&((section_info + section_index)->key_info + key_index)->string);
	}
	*((section_info + section_index)->key_info + key_index)->key_name = '\0';
	((section_info + section_index)->key_info + key_index)->string = NULL;
	return 1;
}

/*
 * profile_find_section - check if section exists (GJC)
 */
int comn_FindProfileSection(const char *section_name)
{
	if (section_find(section_name) == -1) {
		return 0;
	}
	return 1;
}

/*
 * profile_clear_section - delete all keys from profile section (GJC)
 */
int comn_ClearProfileSection(const char *section_name)
{
	SECTION_INFO *section_to_del;
	int j;
	int section_index = section_find(section_name);

	if (section_index == -1) {
		return 0;
	}
	section_to_del = (section_info + section_index);
	if (section_to_del->key_info == NULL) {
		return 0;
	}

	for (j = 0; j < section_to_del->key_count; j++) {
		*(section_to_del->key_info + j)->key_name = '\0';
		if ((section_to_del->key_info + j)->string != NULL) {
			mem_free((void **)&(section_to_del->key_info + j)->string);
		}
	}
	mem_free((void **)&section_to_del->key_info);
	section_to_del->key_info = NULL;
	section_to_del->key_count = 0;
	section_to_del->key_size = 0;

	return 1;
}

/* End of source */
