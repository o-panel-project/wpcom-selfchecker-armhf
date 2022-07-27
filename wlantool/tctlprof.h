/*
 * $Id: tctlprof.h 2 2011-09-20 07:46:40Z r-hara $
 */

#ifndef __TCTLPROF_H__
#define __TCTLPROF_H__

#ifdef __cplusplus
extern "C" {
#endif

extern int comn_ProfileSetup(const char *);		/* INI file */
extern int comn_GetProfileString(
			const char *,		/* section */
			const char *,		/* key */
			const char *,		/* default */
			char *, int,		/* return buf, size */
			const char *);		/* INI file (unused) */
extern int comn_GetProfileInt(
			const char *,		/* section */
			const char *,		/* key */
			const int,			/* default */
			const char *);		/* INI file (unused) */
extern int comn_ProfileTerm();
extern int comn_ProfileFlush(const char *);		/* INI file */
extern int comn_PutProfileString(
			const char *,		/* section */
			const char *,		/* key */
			const char *);		/* string */
extern int comn_PutProfileInt(
			const char *,		/* section */
			const char *,		/* key */
			const int);			/* value */
extern int comn_DelProfileKey(
			const char *,		/* section */
			const char *);		/* key */
extern int comn_FindProfileSection(const char *);  /* section */
extern int comn_ClearProfileSection(const char *); /* section */

#ifdef __cplusplus
}
#endif

#endif /* __TCTLPROF_H__ */
