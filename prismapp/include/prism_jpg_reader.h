///////////////////////////////////////////////////////////////////////////////
//
//            Copyright (c) 2010 by Blue Water Embedded, Inc.
//
//  This software is copyrighted by and is the sole property of Blue Water
//  Embedded, Inc. All rights, title, ownership, or other intersts in the
//  software remain the property of Blue Water Embedded, Inc. This software
//  may only be used in accordance with the corresponding license agreement.
//  Any unauthorized use, duplications, transmission, distribution, or
//  disclosure of this software is expressly forbidden.
//
//  This Copyright notice may not be removed or modified without prior
//  written consent of Blue Water Embedded, Inc.
//
//  Blue Water Embedded, Inc. reserves the right to modify this software
//  without notice.
//
//  Blue Water Embedded, Inc.               info@bwembedded.com
//  3847 Pine Grove Ave                     http://www.bwembedded.com
//  Suite A
//  Fort Gratiot, MI 48059
//
///////////////////////////////////////////////////////////////////////////////


///////////////////////////////////////////////////////////////////////////////
//
//  prism_jpg_reader.h - Prism jpg file reader class definition
//
//  Author: Kenneth G. Maxwell
//
//  Revision History:
//  Refer to SVN revision history.
// 
///////////////////////////////////////////////////////////////////////////////

///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////

///////////////////////////////////////////////////////////////////////////////
// NOTES:
//
// This software module originated as "libjpg", a free software library that
// is published by the Independent JPEG Group. They request that the following
// be included in any software that incorporates this source code:

/*
This package contains C software to implement JPEG image encoding, decoding,
and transcoding.  JPEG is a standardized compression method for full-color
and gray-scale images.

The distributed programs provide conversion between JPEG "JFIF" format and
image files in PBMPLUS PPM/PGM, GIF, BMP, and Targa file formats.  The
core compression and decompression library can easily be reused in other
programs, such as image viewers.  The package is highly portable C code;
we have tested it on many machines ranging from PCs to Crays.

We are releasing this software for both noncommercial and commercial use.
Companies are welcome to use it as the basis for JPEG-related products.
We do not ask a royalty, although we do ask for an acknowledgement in
product literature (see the README file in the distribution for details).
We hope to make this software industrial-quality --- although, as with
anything that's free, we offer no warranty and accept no liability.

For more information, contact jpeg-info@uc.ag.

*/
//
// The original code can also be downloaded here:
//
// http://www.ijg.org/files/jpegsr8c.zip
//
// The Pm_Jpg_Reader class is a wrapper that adds the Prism FileIO abstraction
// layer, making it easy to compile and run this code on different
// operating systems.
//
///////////////////////////////////////////////////////////////////////////////

#ifdef PRISM_RUNTIME_JPG_READER

#ifndef _PRISM_JPG_READER_
#define _PRISM_JPG_READER_

BEGIN_NAMESPACE_PRISM

#ifndef PRISM_DEFAULT_MAX_JPRISM_MEM
#define PRISM_DEFAULT_MAX_JPRISM_MEM         0x00010000  
#endif

#define JPRISM_INTERNALS
#define JCONFIG_INCLUDED    

#include <stddef.h>


#define MEMZERO(target,size)    memset((void *)(target), 0, (size_t)(size))
#define MEMCOPY(dest,src,size)    memcpy((void *)(dest), (const void *)(src), (size_t)(size))
#define SIZEOF(object)    ((size_t) sizeof(object))
#define BITS_IN_JSAMPLE  8   

#define MAX_COMPONENTS  8  

#define MAXuint8_t    255
#define CENTERuint8_t    128

#define GETuint8_t(value)  (value)

#define JPRISM_MAX_DIMENSION  65500L  
#define MAX_JPG_IMAGE_WIDTH     8000
#define MAX_JPG_IMAGE_HEIGHT    6000


#define METHODDEF(type)        static type
#define LOCAL(type)        static type
#define GLOBAL(type)        type
#define EXTERN(type)        extern type

#define JMETHOD(type,methodname,arglist)  type (*methodname) arglist

#ifdef JPRISM_INTERNALS
#define JPRISM_INTERNAL_OPTIONS
#endif

#ifdef JPRISM_INTERNAL_OPTIONS

#define DCT_ISLOW_SUPPORTED  

#ifdef PRISM_JPG_VIRTUAL_MEMORY

#define D_MULTISCAN_FILES_SUPPORTED 
#define D_PROGRESSIVE_SUPPORTED

#endif


#define RGB_RED        0  
#define RGB_GREEN    1    
#define RGB_BLUE    2    
#define RGB_PIXELSIZE    3   

#define INLINE         

#ifndef MULTIPLIER
#define MULTIPLIER  int    
#endif

#endif

#define JPRISM_LIB_VERSION  62 


#define DCTSIZE            8  
#define DCTSIZE2           64 
#define NUM_QUANT_TBLS      4  
#define NUM_HUPM_BORDER_TBLS       4    
#define NUM_ARITH_TBLS      16  
#define MAX_COMPS_IN_SCAN   4   
#define MAX_SAMP_FACTOR     4  

#ifndef D_MAX_BLOCKS_IN_MCU
#define D_MAX_BLOCKS_IN_MCU   10 
#endif


typedef uint8_t  *JSAMPROW;    
typedef JSAMPROW *JSAMPARRAY;   
typedef JSAMPARRAY *JSAMPIMAGE; 

typedef pm_int_t JBLOCK[DCTSIZE2];  
typedef JBLOCK  *JBLOCKROW;   
typedef JBLOCKROW *JBLOCKARRAY;  
typedef JBLOCKARRAY *JBLOCKIMAGE; 

typedef pm_int_t  *pm_int_tPTR;  


typedef struct {

  pm_uint_t quantval[DCTSIZE2];    
} JQUANT_TBL;



typedef struct {
  uint8_t bits[17];
  uint8_t huffval[256];        
} JHUPM_BORDER_TBL;




typedef struct {
  int component_id;        
  int component_index;
  int h_samp_factor;
  int v_samp_factor; 
  int quant_tbl_no;   

  int dc_tbl_no; 
  int ac_tbl_no;      
  
  pm_uint_t width_in_blocks;
  pm_uint_t height_in_blocks;

  int DCT_scaled_size;

  pm_uint_t downsampled_width;    
  pm_uint_t downsampled_height;

  pm_bool_t component_needed; 

  int MCU_width;
  int MCU_height;
  int MCU_blocks; 
  int MCU_sample_width;       
  int last_col_width; 
  int last_row_height;

  JQUANT_TBL * quant_table;
  void * dct_table;
} jpeg_component_info;

typedef struct {
  int comps_in_scan;      
  int component_index[MAX_COMPS_IN_SCAN];
  int Ss, Se;          
  int Ah, Al;    
} jpeg_scan_info;


typedef struct jpeg_marker_struct  * jpeg_saved_marker_ptr;

struct jpeg_marker_struct {
  jpeg_saved_marker_ptr next;  
  uint8_t marker;         
  unsigned int original_length;  
  unsigned int data_length; 
  uint8_t  * data;        

};


typedef enum {
    JCS_UNKNOWN,      
    JCS_GRAYSCALE,
    JCS_RGB,        
    JCS_YCbCr,    
    JCS_CMYK,      
    JCS_YCCK   
} J_COLOR_SPACE;


typedef enum {
    JDCT_ISLOW,     
    JDCT_IFAST,     
    JDCT_FLOAT     
} J_DCT_METHOD;

#ifndef JDCT_DEFAULT      
#define JDCT_DEFAULT  JDCT_ISLOW
#endif

#ifndef JDCT_FASTEST 
#define JDCT_FASTEST  JDCT_IFAST
#endif

typedef enum {
    JDITHER_NONE,       
    JDITHER_ORDERED,    
    JDITHER_FS      
} J_DITHER_MODE;

typedef struct jpeg_decompress_struct * j_decompress_ptr;

class Pm_Graphic_Reader;      
struct jpeg_decompress_struct {

  struct jpeg_error_mgr * err;  \
  struct jpeg_memory_mgr * mem;    \
  struct jpeg_progress_mgr * progress; \
  void * client_data;        \
  pm_bool_t is_decompressor;  \
  int global_state;       

  struct jpeg_source_mgr * src;

  pm_uint_t image_width;    
  pm_uint_t image_height;  
  int num_components;      
  J_COLOR_SPACE jpeg_color_space; 

  J_COLOR_SPACE out_color_space; 

  unsigned int scale_num, scale_denom;

  pm_bool_t buffered_image; 
  pm_bool_t raw_data_out;

  J_DCT_METHOD dct_method;   
  pm_bool_t do_fancy_upsampling; 
  pm_bool_t do_block_smoothing; 

  pm_uint_t output_width;  
  pm_uint_t output_height; 
  int out_color_components;
  int output_components;

  int rec_outbuf_height;

  int actual_number_of_colors;  
  JSAMPARRAY colormap; 

  pm_uint_t output_scanline;

  int input_scan_number; 
  pm_uint_t input_iMCU_row;

  int output_scan_number;  
  pm_uint_t output_iMCU_row; 

  int (*coef_bits)[DCTSIZE2]; 

  JQUANT_TBL * quant_tbl_ptrs[NUM_QUANT_TBLS];

  JHUPM_BORDER_TBL * dc_huff_tbl_ptrs[NUM_HUPM_BORDER_TBLS];
  JHUPM_BORDER_TBL * ac_huff_tbl_ptrs[NUM_HUPM_BORDER_TBLS];

  int data_precision; 

  jpeg_component_info * comp_info;

  pm_bool_t progressive_mode;
  pm_bool_t arith_code;

  uint8_t arith_dc_L[NUM_ARITH_TBLS]; 
  uint8_t arith_dc_U[NUM_ARITH_TBLS];
  uint8_t arith_ac_K[NUM_ARITH_TBLS]; 

  unsigned int restart_interval;
 
  pm_bool_t saw_JFIF_marker;

  uint8_t JFIF_major_version;
  uint8_t JFIF_minor_version;
  uint8_t density_unit;       
  pm_uint_t X_density;      
  pm_uint_t Y_density;      
  pm_bool_t saw_Adobe_marker;  
  uint8_t Adobe_transform; 

  pm_bool_t CCIR601_sampling; 

  jpeg_saved_marker_ptr marker_list; 

  int max_h_samp_factor;  
  int max_v_samp_factor; 

  int min_DCT_scaled_size; 

  pm_uint_t total_iMCU_rows;

  uint8_t * sample_range_limit; 

  int comps_in_scan; 
  jpeg_component_info * cur_comp_info[MAX_COMPS_IN_SCAN];

  pm_uint_t MCUs_per_row; 
  pm_uint_t MCU_rows_in_scan;

  int blocks_in_MCU;     
  int MCU_membership[D_MAX_BLOCKS_IN_MCU];

  int Ss, Se, Ah, Al;

  int unread_marker;


  struct jpeg_decomp_master * master;
  struct jpeg_d_main_controller * main;
  struct jpeg_d_coef_controller * coef;
  struct jpeg_d_post_controller * post;
  struct jpeg_input_controller * inputctl;
  struct jpeg_marker_reader * marker;
  struct jpeg_entropy_decoder * entropy;
  struct jpeg_inverse_dct * idct;
  struct jpeg_upsampler * upsample;
  struct jpeg_color_deconverter * cconvert;
  struct jpeg_color_quantizer * cquantize;
  Pm_Graphic_Reader *pReader;
};


struct jpeg_error_mgr {
  JMETHOD(void, error_exit, (j_decompress_ptr cinfo));
  JMETHOD(void, emit_message, (j_decompress_ptr cinfo, int msg_level));
  JMETHOD(void, output_message, (j_decompress_ptr cinfo));
  JMETHOD(void, format_message, (j_decompress_ptr cinfo, int8_t * buffer));
#define JMSG_LENGTH_MAX  200    
  JMETHOD(void, reset_error_mgr, (j_decompress_ptr cinfo));
  

  int msg_code;
#define JMSG_STR_PARM_MAX  80
  union {
    int i[8];
    int8_t s[JMSG_STR_PARM_MAX];
  } msg_parm;
    
  int trace_level; 
  long num_warnings; 

  const int8_t * const * jpeg_message_table;
  int last_jpeg_message; 

  const int8_t * const * addon_message_table; 
  int first_addon_message; 
  int last_addon_message;
};

struct jpeg_progress_mgr {
  JMETHOD(void, progress_monitor, (j_decompress_ptr cinfo));

  long pass_counter; 
  long pass_limit; 
  int completed_passes; 
  int total_passes; 
};

struct jpeg_source_mgr {
  const uint8_t * next_input_byte;
  size_t bytes_in_buffer; 

  uint8_t * buffer;
  pm_bool_t start_of_file;

  JMETHOD(void, init_source, (j_decompress_ptr cinfo));
  JMETHOD(pm_bool_t, fill_input_buffer, (j_decompress_ptr cinfo));
  JMETHOD(void, skip_input_data, (j_decompress_ptr cinfo, long num_bytes));
  JMETHOD(pm_bool_t, resync_to_restart, (j_decompress_ptr cinfo, int desired));
  JMETHOD(void, term_source, (j_decompress_ptr cinfo));
};


#define JPOOL_PERMANENT    0   
#define JPOOL_IMAGE    1   
#define JPOOL_NUMPOOLS    2

typedef struct jvirt_sarray_control * jvirt_sarray_ptr;
typedef struct jvirt_barray_control * jvirt_barray_ptr;

#ifndef ALIGN_TYPE      
#define ALIGN_TYPE  double
#endif


typedef union small_pool_struct * small_pool_ptr;

typedef union small_pool_struct {
  struct {
    small_pool_ptr next;  
    size_t bytes_used;     
    size_t bytes_left;  
  } hdr;
  ALIGN_TYPE dummy;   
} small_pool_hdr;

typedef union large_pool_struct  * large_pool_ptr;

typedef union large_pool_struct {
  struct {
    large_pool_ptr next;   
    size_t bytes_used;    
    size_t bytes_left;    
  } hdr;
  ALIGN_TYPE dummy;  
} large_pool_hdr;



struct jpeg_memory_mgr {
  JMETHOD(void *, alloc_small, (j_decompress_ptr cinfo, int pool_id,
                size_t sizeofobject));
  JMETHOD(void  *, alloc_large, (j_decompress_ptr cinfo, int pool_id,
                     size_t sizeofobject));
  JMETHOD(JSAMPARRAY, alloc_sarray, (j_decompress_ptr cinfo, int pool_id,
                     pm_uint_t samplesperrow,
                     pm_uint_t numrows));
  JMETHOD(JBLOCKARRAY, alloc_barray, (j_decompress_ptr cinfo, int pool_id,
                      pm_uint_t blocksperrow,
                      pm_uint_t numrows));
  JMETHOD(jvirt_sarray_ptr, request_virt_sarray, (j_decompress_ptr cinfo,
                          int pool_id,
                          pm_bool_t pre_zero,
                          pm_uint_t samplesperrow,
                          pm_uint_t numrows,
                          pm_uint_t maxaccess));
  JMETHOD(jvirt_barray_ptr, request_virt_barray, (j_decompress_ptr cinfo,
                          int pool_id,
                          pm_bool_t pre_zero,
                          pm_uint_t blocksperrow,
                          pm_uint_t numrows,
                          pm_uint_t maxaccess));
  JMETHOD(void, realize_virt_arrays, (j_decompress_ptr cinfo));
  JMETHOD(JSAMPARRAY, access_virt_sarray, (j_decompress_ptr cinfo,
                       jvirt_sarray_ptr ptr,
                       pm_uint_t start_row,
                       pm_uint_t num_rows,
                       pm_bool_t writable));
  JMETHOD(JBLOCKARRAY, access_virt_barray, (j_decompress_ptr cinfo,
                        jvirt_barray_ptr ptr,
                        pm_uint_t start_row,
                        pm_uint_t num_rows,
                        pm_bool_t writable));
  JMETHOD(void, free_pool, (j_decompress_ptr cinfo, int pool_id));
  JMETHOD(void, self_destruct, (j_decompress_ptr cinfo));

  long max_memory_to_use;

  long max_alloc_chunk;

  small_pool_ptr small_list[JPOOL_NUMPOOLS];
  large_pool_ptr large_list[JPOOL_NUMPOOLS];
  jvirt_sarray_ptr virt_sarray_list;
  jvirt_barray_ptr virt_barray_list;

  long total_space_allocated;
  pm_uint_t last_rowsperchunk;
};


typedef JMETHOD(pm_bool_t, jpeg_marker_parser_method, (j_decompress_ptr cinfo));


EXTERN(struct jpeg_error_mgr *) jpeg_std_error
    (struct jpeg_error_mgr * err);

#define jpeg_create_decompress(cinfo) \
    jpeg_CreateDecompress((cinfo), JPRISM_LIB_VERSION, \
              (size_t) sizeof(struct jpeg_decompress_struct))
EXTERN(void) jpeg_CreateDecompress (j_decompress_ptr cinfo,
                    int version, size_t structsize);
EXTERN(void) jpeg_destroy_decompress (j_decompress_ptr cinfo);

EXTERN(void) jpeg_stdio_src (j_decompress_ptr cinfo);

EXTERN(JQUANT_TBL *) jpeg_alloc_quant_table (j_decompress_ptr cinfo);
EXTERN(JHUPM_BORDER_TBL *) jpeg_alloc_huff_table (j_decompress_ptr cinfo);

EXTERN(int) jpeg_read_header (j_decompress_ptr cinfo,
                  pm_bool_t require_image);
#define JPRISM_SUSPENDED        0 
#define JPRISM_HEADER_OK        1 
#define JPRISM_HEADER_TABLES_ONLY    2

EXTERN(pm_bool_t) jpeg_start_decompress (j_decompress_ptr cinfo);
EXTERN(pm_uint_t) jpeg_read_scanlines (j_decompress_ptr cinfo,
                        JSAMPARRAY scanlines,
                        pm_uint_t max_lines);
EXTERN(pm_bool_t) jpeg_finish_decompress (j_decompress_ptr cinfo);

EXTERN(pm_uint_t) jpeg_read_raw_data (j_decompress_ptr cinfo,
                       JSAMPIMAGE data,
                       pm_uint_t max_lines);

EXTERN(pm_bool_t) jpeg_has_multiple_scans (j_decompress_ptr cinfo);
EXTERN(pm_bool_t) jpeg_start_output (j_decompress_ptr cinfo,
                       int scan_number);
EXTERN(pm_bool_t) jpeg_finish_output (j_decompress_ptr cinfo);
EXTERN(pm_bool_t) jpeg_input_complete (j_decompress_ptr cinfo);
EXTERN(void) jpeg_new_colormap (j_decompress_ptr cinfo);
EXTERN(int) jpeg_consume_input (j_decompress_ptr cinfo);

#define JPRISM_REACHED_SOS    1 
#define JPRISM_REACHED_EOI    2 
#define JPRISM_ROW_COMPLETED    3 
#define JPRISM_SCAN_COMPLETED    4 

EXTERN(void) jpeg_calc_output_dimensions (j_decompress_ptr cinfo);

EXTERN(void) jpeg_save_markers
    (j_decompress_ptr cinfo, int marker_code,
         unsigned int length_limit);

EXTERN(void) jpeg_set_marker_processor
    (j_decompress_ptr cinfo, int marker_code,
         jpeg_marker_parser_method routine);

EXTERN(jvirt_barray_ptr *) jpeg_read_coefficients (j_decompress_ptr cinfo);

EXTERN(void) jpeg_abort_decompress (j_decompress_ptr cinfo);

EXTERN(void) jpeg_abort (j_decompress_ptr cinfo);
EXTERN(void) jpeg_destroy (j_decompress_ptr cinfo);

EXTERN(pm_bool_t) jpeg_resync_to_restart (j_decompress_ptr cinfo,
                        int desired);

#define JPRISM_RST0    0xD0 
#define JPRISM_EOI    0xD9 
#define JPRISM_APP0    0xE0
#define JPRISM_COM    0xFE  

#ifdef JPRISM_INTERNALS

typedef enum {           
    JBUF_PASS_THRU,       
    JBUF_SAVE_SOURCE,    
    JBUF_CRANK_DEST,  
    JBUF_SAVE_AND_PASS  
} J_BUF_MODE;

#define CSTATE_START    100 
#define CSTATE_SCANNING    101  
#define CSTATE_RAW_OK    102 
#define CSTATE_WRCOEFS    103 
#define DSTATE_START    200  
#define DSTATE_INHEADER    201  
#define DSTATE_READY    202 
#define DSTATE_PRELOAD    203   
#define DSTATE_PRESCAN    204  
#define DSTATE_SCANNING    205   
#define DSTATE_RAW_OK    206   
#define DSTATE_BUFIMAGE    207 
#define DSTATE_BUFPOST    208
#define DSTATE_RDCOEFS    209   
#define DSTATE_STOPPING    210 


struct jpeg_decomp_master {
  JMETHOD(void, prepare_for_output_pass, (j_decompress_ptr cinfo));
  JMETHOD(void, finish_output_pass, (j_decompress_ptr cinfo));

  pm_bool_t is_dummy_pass;

  int pass_number; 

  pm_bool_t using_merged_upsample;

  struct jpeg_color_quantizer * quantizer_1pass;
  struct jpeg_color_quantizer * quantizer_2pass;
};

struct jpeg_input_controller {
  JMETHOD(int, consume_input, (j_decompress_ptr cinfo));
  JMETHOD(void, reset_input_controller, (j_decompress_ptr cinfo));
  JMETHOD(void, start_input_pass, (j_decompress_ptr cinfo));
  JMETHOD(void, finish_input_pass, (j_decompress_ptr cinfo));

  pm_bool_t has_multiple_scans;
  pm_bool_t eoi_reached; 
  pm_bool_t inheaders;   
};

struct jpeg_d_main_controller {
  JMETHOD(void, start_pass, (j_decompress_ptr cinfo, J_BUF_MODE pass_mode));
  JMETHOD(void, process_data, (j_decompress_ptr cinfo,
                   JSAMPARRAY output_buf, pm_uint_t *out_row_ctr,
                   pm_uint_t out_rows_avail));

  JSAMPARRAY buffer[MAX_COMPONENTS];

  pm_bool_t buffer_full; 
  pm_uint_t rowgroup_ctr;

  JSAMPIMAGE xbuffer[2];

  int whichptr;   
  int context_state;    
  pm_uint_t rowgroups_avail;
  pm_uint_t iMCU_row_ctr; 

};

struct jpeg_d_coef_controller {
  JMETHOD(void, start_input_pass, (j_decompress_ptr cinfo));
  JMETHOD(int, consume_data, (j_decompress_ptr cinfo));
  JMETHOD(void, start_output_pass, (j_decompress_ptr cinfo));
  JMETHOD(int, decompress_data, (j_decompress_ptr cinfo,
                 JSAMPIMAGE output_buf));
  jvirt_barray_ptr *coef_arrays;


  pm_uint_t MCU_ctr;        
  int MCU_vert_offset;      
  int MCU_rows_per_iMCU_row; 
  JBLOCKROW MCU_buffer[D_MAX_BLOCKS_IN_MCU];

#ifdef D_MULTISCAN_FILES_SUPPORTED
  jvirt_barray_ptr whole_image[MAX_COMPONENTS];
#endif
};

struct jpeg_d_post_controller {
  JMETHOD(void, start_pass, (j_decompress_ptr cinfo, J_BUF_MODE pass_mode));
  JMETHOD(void, post_process_data, (j_decompress_ptr cinfo,
                    JSAMPIMAGE input_buf,
                    pm_uint_t *in_row_group_ctr,
                    pm_uint_t in_row_groups_avail,
                    JSAMPARRAY output_buf,
                    pm_uint_t *out_row_ctr,
                    pm_uint_t out_rows_avail));

  jvirt_sarray_ptr whole_image;    
  JSAMPARRAY buffer;     
  pm_uint_t strip_height;  
  pm_uint_t starting_row;
  pm_uint_t next_row;

};

struct jpeg_marker_reader {
  JMETHOD(void, reset_marker_reader, (j_decompress_ptr cinfo));

  JMETHOD(int, read_markers, (j_decompress_ptr cinfo));
  jpeg_marker_parser_method read_restart_marker;

  pm_bool_t saw_SOI;
  pm_bool_t saw_SOF;
  int next_restart_num; 
  unsigned int discarded_bytes; 

  jpeg_marker_parser_method process_COM;
  jpeg_marker_parser_method process_APPn[16];

  unsigned int length_limit_COM;
  unsigned int length_limit_APPn[16];

  jpeg_saved_marker_ptr cur_marker;
  unsigned int bytes_read;
};

typedef int32_t bit_buf_type; 
#define BIT_BUF_SIZE  32


typedef struct {   
  bit_buf_type get_buffer;
  int bits_left;     
} bitread_perm_state;

typedef struct {  
  const uint8_t * next_input_byte;
  size_t bytes_in_buffer;

  bit_buf_type get_buffer;
  int bits_left; 
  j_decompress_ptr cinfo;
} bitread_working_state;

#define BITREAD_STATE_VARS  \
    register bit_buf_type get_buffer;  \
    register int bits_left;  \
    bitread_working_state br_state

#define BITREAD_LOAD_STATE(cinfop,permstate)  \
    br_state.cinfo = cinfop; \
    br_state.next_input_byte = cinfop->src->next_input_byte; \
    br_state.bytes_in_buffer = cinfop->src->bytes_in_buffer; \
    get_buffer = permstate.get_buffer; \
    bits_left = permstate.bits_left;

#define BITREAD_SAVE_STATE(cinfop,permstate)  \
    cinfop->src->next_input_byte = br_state.next_input_byte; \
    cinfop->src->bytes_in_buffer = br_state.bytes_in_buffer; \
    permstate.get_buffer = get_buffer; \
    permstate.bits_left = bits_left

#define CHECK_BIT_BUFFER(state,nbits,action) \
    { if (bits_left < (nbits)) {  \
        if (! jpeg_fill_bit_buffer(&(state),get_buffer,bits_left,nbits))  \
          { action; }  \
        get_buffer = (state).get_buffer; bits_left = (state).bits_left; } }

#define GET_BITS(nbits) \
    (((int) (get_buffer >> (bits_left -= (nbits)))) & ((1<<(nbits))-1))

#define PEEK_BITS(nbits) \
    (((int) (get_buffer >> (bits_left -  (nbits)))) & ((1<<(nbits))-1))

#define DROP_BITS(nbits) \
    (bits_left -= (nbits))


typedef struct {
  int last_dc_val[MAX_COMPS_IN_SCAN];
  unsigned int EOBRUN;
} savable_state;

#define HUPM_BORDER_LOOKAHEAD    8

typedef struct {
  int32_t maxcode[18]; 
  int32_t valoffset[17];

  JHUPM_BORDER_TBL *pub;

  int look_nbits[1<<HUPM_BORDER_LOOKAHEAD]; 
  uint8_t look_sym[1<<HUPM_BORDER_LOOKAHEAD];
} d_derived_tbl;


struct jpeg_entropy_decoder {
  JMETHOD(void, start_pass, (j_decompress_ptr cinfo));
  JMETHOD(pm_bool_t, decode_mcu, (j_decompress_ptr cinfo,
                JBLOCKROW *MCU_data));

  pm_bool_t insufficient_data;

  bitread_perm_state bitstate; 
  savable_state saved;

  unsigned int restarts_to_go;


  d_derived_tbl * dc_derived_tbls[NUM_HUPM_BORDER_TBLS];
  d_derived_tbl * ac_derived_tbls[NUM_HUPM_BORDER_TBLS];

  d_derived_tbl * derived_tbls[NUM_HUPM_BORDER_TBLS];
  d_derived_tbl * ac_derived_tbl;

  d_derived_tbl * dc_cur_tbls[D_MAX_BLOCKS_IN_MCU];
  d_derived_tbl * ac_cur_tbls[D_MAX_BLOCKS_IN_MCU];

  pm_bool_t dc_needed[D_MAX_BLOCKS_IN_MCU];
  pm_bool_t ac_needed[D_MAX_BLOCKS_IN_MCU];
};

typedef JMETHOD(void, inverse_DCT_method_ptr,
        (j_decompress_ptr cinfo, jpeg_component_info * compptr,
         JBLOCK *coef_block,
         JSAMPARRAY output_buf, pm_uint_t output_col));

struct jpeg_inverse_dct {
  JMETHOD(void, start_pass, (j_decompress_ptr cinfo));
  inverse_DCT_method_ptr inverse_DCT[MAX_COMPONENTS];
  int cur_method[MAX_COMPONENTS];
};

typedef JMETHOD(void, upsample1_ptr,
        (j_decompress_ptr cinfo, jpeg_component_info * compptr,
         JSAMPARRAY input_data, JSAMPARRAY * output_data_ptr));

struct jpeg_upsampler {
  JMETHOD(void, start_pass, (j_decompress_ptr cinfo));
  JMETHOD(void, upsample, (j_decompress_ptr cinfo,
               JSAMPIMAGE input_buf,
               pm_uint_t *in_row_group_ctr,
               pm_uint_t in_row_groups_avail,
               JSAMPARRAY output_buf,
               pm_uint_t *out_row_ctr,
               pm_uint_t out_rows_avail));

  pm_bool_t need_context_rows;

  JSAMPARRAY color_buf[MAX_COMPONENTS];

  upsample1_ptr methods[MAX_COMPONENTS];

  int next_row_out;
  pm_uint_t rows_to_go;

  int rowgroup_height[MAX_COMPONENTS];
  uint8_t h_expand[MAX_COMPONENTS];
  uint8_t v_expand[MAX_COMPONENTS];
};

struct jpeg_color_deconverter {
  JMETHOD(void, start_pass, (j_decompress_ptr cinfo));
  JMETHOD(void, color_convert, (j_decompress_ptr cinfo,
                JSAMPIMAGE input_buf, pm_uint_t input_row,
                JSAMPARRAY output_buf, int num_rows));

  int * Cr_r_tab;    
  int * Cb_b_tab; 
  int32_t * Cr_g_tab; 
  int32_t * Cb_g_tab; 

};

struct jpeg_color_quantizer {
  JMETHOD(void, start_pass, (j_decompress_ptr cinfo, pm_bool_t is_pre_scan));
  JMETHOD(void, color_quantize, (j_decompress_ptr cinfo,
                 JSAMPARRAY input_buf, JSAMPARRAY output_buf,
                 int num_rows));
  JMETHOD(void, finish_pass, (j_decompress_ptr cinfo));
  JMETHOD(void, new_color_map, (j_decompress_ptr cinfo));
};


#undef MAX
#define MAX(a,b)    ((a) > (b) ? (a) : (b))
#undef MIN
#define MIN(a,b)    ((a) < (b) ? (a) : (b))


#define SHIFT_TEMPS
#define RIGHT_SHIFT(x,shft)    ((x) >> (shft))

EXTERN(void) jinit_master_decompress (j_decompress_ptr cinfo);
EXTERN(void) jinit_d_main_controller (j_decompress_ptr cinfo,
                      pm_bool_t need_full_buffer);
EXTERN(pm_bool_t) jinit_d_coef_controller (j_decompress_ptr cinfo,
                      pm_bool_t need_full_buffer);
EXTERN(void) jinit_d_post_controller (j_decompress_ptr cinfo,
                      pm_bool_t need_full_buffer);
EXTERN(void) jinit_input_controller (j_decompress_ptr cinfo);
EXTERN(void) jinit_marker_reader (j_decompress_ptr cinfo);
EXTERN(void) jinit_huff_decoder (j_decompress_ptr cinfo);
EXTERN(void) jinit_phuff_decoder (j_decompress_ptr cinfo);
EXTERN(void) jinit_inverse_dct (j_decompress_ptr cinfo);
EXTERN(void) jinit_upsampler (j_decompress_ptr cinfo);
EXTERN(void) jinit_color_deconverter (j_decompress_ptr cinfo);
EXTERN(void) jinit_1pass_quantizer (j_decompress_ptr cinfo);
EXTERN(void) jinit_2pass_quantizer (j_decompress_ptr cinfo);
EXTERN(void) jinit_merged_upsampler (j_decompress_ptr cinfo);
EXTERN(void) jinit_memory_mgr (j_decompress_ptr cinfo);

EXTERN(long) jdiv_round_up (long a, long b);
EXTERN(long) jround_up (long a, long b);

EXTERN(void) jcopy_sample_rows (JSAMPARRAY input_array, int source_row,
                    JSAMPARRAY output_array, int dest_row,
                    int num_rows, pm_uint_t num_cols);
EXTERN(void) jcopy_block_row (JBLOCKROW input_row, JBLOCKROW output_row,
                  pm_uint_t num_blocks);
EXTERN(void) jzero_ (void  * target, size_t bytestozero);
extern const int jpeg_natural_order[]; 

void jzero_far (void  * target, size_t bytestozero);

#ifndef JERROR_H
#define JERROR_H

#define ERREXIT(cinfo,code)  
#define ERREXIT1(cinfo,code,p1)  
#define ERREXIT2(cinfo,code,p1,p2)  
#define ERREXIT3(cinfo,code,p1,p2,p3)  
#define ERREXIT4(cinfo,code,p1,p2,p3,p4)  
#define ERREXITS(cinfo,code,str)  
#define MAKESTMT(stuff)        do { stuff } while (0)
#define WARNMS(cinfo,code)  
#define WARNMS1(cinfo,code,p1)  
#define WARNMS2(cinfo,code,p1,p2)  
#define TRACEMS(cinfo,lvl,code)  
#define TRACEMS1(cinfo,lvl,code,p1)  
#define TRACEMS2(cinfo,lvl,code,p1,p2)  
#define TRACEMS3(cinfo,lvl,code,p1,p2,p3)  
#define TRACEMS4(cinfo,lvl,code,p1,p2,p3,p4)  
#define TRACEMS5(cinfo,lvl,code,p1,p2,p3,p4,p5)  
#define TRACEMS8(cinfo,lvl,code,p1,p2,p3,p4,p5,p6,p7,p8)  
#define TRACEMSS(cinfo,lvl,code,str)  
#endif

#endif 


///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
class Pm_Jpg_Reader : public Pm_Graphic_Reader
{
    public:

        Pm_Jpg_Reader(pm_uint_t Id = 0);
        virtual ~Pm_Jpg_Reader(void);

       #ifdef PRISM_GRAPHIC_READER_FILEIO
        virtual pm_bool_t GetGraphicInfo(Pm_File *Src, pm_graphic_info_t *info);
        virtual pm_bool_t ReadImage(Pm_File *fd, pm_int_t Count = 1);
       #else
        virtual pm_bool_t ReadImage(pm_int_t Count = 1);
       #endif


    protected:

    private:

        void StoreUncompressedData(pm_bitmap_t *pMap, pm_uint_t Row);

        jpeg_decompress_struct mJpgInfo;
        uint8_t *mpBuffer;
};

END_NAMESPACE_PRISM

#endif 
#endif 


