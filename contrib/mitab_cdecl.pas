{**********************************************************************
 * $Id: mitab_cdecl.pas,v 1.1 2002-02-22 13:43:30 daniel Exp $
 *
 * Name:     mitab_cdecl.pas
 * Project:  MapInfo TAB Read/Write library
 * Language: Pascal
 * Purpose:  Pascal interface unit for MITAB API prototypes using CDECL
 *           calling convention. Used with MITAB dll compiled without
 *           the /Gz qualifier.
 * Author:   Bo Thomsen, bvt@sns.dk
 *
 **********************************************************************
 * Copyright (c) 2002, Bo Thomsen
 *
 * Permission is hereby granted, free of charge, to any person obtaining a
 * copy of this software and associated documentation files (the "Software"),
 * to deal in the Software without restriction, including without limitation
 * the rights to use, copy, modify, merge, publish, distribute, sublicense,
 * and/or sell copies of the Software, and to permit persons to whom the
 * Software is furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included
 * in all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.  IN NO EVENT SHALL
 * THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING
 * FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER
 * DEALINGS IN THE SOFTWARE.
 **********************************************************************
 *
 * $Log: mitab_cdecl.pas,v $
 * Revision 1.1  2002-02-22 13:43:30  daniel
 * Initial Revision from Bo Thomsen
 *
 * Revision 1.00  2002/02/20 12:35:00  bvt
 * New - used in conjunction with mitab_capi.cpp,v 1.15 compiled without
 * /Gz (cdecl calling convention)
 *
 *}

unit Mitab_cdecl;

interface

Type

  mitab_handle = Longword;
  mitab_feature = Longword;
  mitab_projinfo = Longword;

const

// feature type values
  TABFC_NoGeom      = 0;
  TABFC_Point       = 1;
  TABFC_FontPoint   = 2;
  TABFC_CustomPoint = 3;
  TABFC_Text        = 4;
  TABFC_Polyline    = 5;
  TABFC_Arc         = 6;
  TABFC_Region      = 7;
  TABFC_Rectangle   = 8;
  TABFC_Ellipse     = 9;

// field types
  TABFT_Char        = 1;
  TABFT_Integer     = 2;
  TABFT_SmallInt    = 3;
  TABFT_Decimal     = 4;
  TABFT_Float       = 5;
  TABFT_Date        = 6;
  TABFT_Logical     = 7;

// text justification
  TABTJ_Left        = 0;
  TABTJ_Center      = 1;
  TABTJ_Right       = 2;

// text spacing
  TABTS_Single      = 0;
  TABTS_1_5         = 1;
  TABTS_Double      = 2;

// test linetype
  TABTL_NoLine      = 0;
  TABTL_Simple      = 1;
  TABTL_Arrow       = 2;

const

  DLL = 'mitab.dll';

function  mitab_c_add_field              (handle: mitab_handle; field_name: pchar;field_type, width, precision: longint): longint;                              cdecl; external DLL Name 'mitab_c_add_field'               ;
procedure mitab_c_close                  (handle: mitab_handle);                                                                                                cdecl; external DLL Name 'mitab_c_close'                   ;
function  mitab_c_create                 (filename, mif_or_tab, mif_projectiondef: pchar; north, south, east, west: double): mitab_handle;                      cdecl; external DLL Name 'mitab_c_create'                  ;
function  mitab_c_create_feature         (handle: mitab_handle; feature_type: longint): mitab_feature;                                                          cdecl; external DLL Name 'mitab_c_create_feature'          ;
procedure mitab_c_destroy_feature        (feature: mitab_feature);                                                                                              cdecl; external DLL Name 'mitab_c_destroy_feature'         ;
function  mitab_c_get_brush_bgcolor      (feature: mitab_feature): longint;                                                                                     cdecl; external DLL Name 'mitab_c_get_brush_bgcolor'       ;
function  mitab_c_get_brush_fgcolor      (feature: mitab_feature): longint;                                                                                     cdecl; external DLL Name 'mitab_c_get_brush_fgcolor'       ;
function  mitab_c_get_brush_pattern      (feature: mitab_feature): longint;                                                                                     cdecl; external DLL Name 'mitab_c_get_brush_pattern'       ;
function  mitab_c_get_brush_transparent  (feature: mitab_feature): longint;                                                                                     cdecl; external DLL Name 'mitab_c_get_brush_transparent'   ;
function  mitab_c_get_field_as_string_vb (feature: mitab_feature; field: longint; value: pchar; l: longint): longint;                                           cdecl; external DLL Name 'mitab_c_get_field_as_string_vb'  ;
function  mitab_c_get_field_as_string    (feature: mitab_feature; field: longint): pchar;                                                                       cdecl; external DLL Name 'mitab_c_get_field_as_string'     ;
function  mitab_c_get_field_count        (handle: mitab_handle): longint;                                                                                       cdecl; external DLL Name 'mitab_c_get_field_count'         ;
function  mitab_c_get_field_name_vb      (handle: mitab_handle; field: longint; name: pchar; l: longint): longint;                                              cdecl; external DLL Name 'mitab_c_get_field_name_vb'       ;
function  mitab_c_get_field_name         (handle: mitab_handle; field: longint): pchar;                                                                         cdecl; external DLL Name 'mitab_c_get_field_name'          ;
function  mitab_c_get_field_type         (handle: mitab_handle; field: longint): longint;                                                                       cdecl; external DLL Name 'mitab_c_get_field_type'          ;
function  mitab_c_get_font_vb            (feature: mitab_feature; font: pchar; l: longint): longint;                                                            cdecl; external DLL Name 'mitab_c_get_font_vb'             ;
function  mitab_c_get_font               (feature: mitab_feature): pchar;                                                                                       cdecl; external DLL Name 'mitab_c_get_font'                ;
function  mitab_c_get_mif_coordsys_vb    (dataset: mitab_handle; coordsys: pchar; l: longint): longint;                                                         cdecl; external DLL Name 'mitab_c_get_mif_coordsys_vb'     ;
function  mitab_c_get_mif_coordsys       (dataset: mitab_handle): pchar;                                                                                        cdecl; external DLL Name 'mitab_c_get_mif_coordsys'        ;
function  mitab_c_get_parts              (feature: mitab_feature): longint;                                                                                     cdecl; external DLL Name 'mitab_c_get_parts'               ;
function  mitab_c_get_pen_color          (feature: mitab_feature): longint;                                                                                     cdecl; external DLL Name 'mitab_c_get_pen_color'           ;
function  mitab_c_get_pen_pattern        (feature: mitab_feature): longint;                                                                                     cdecl; external DLL Name 'mitab_c_get_pen_pattern'         ;
function  mitab_c_get_pen_width          (feature: mitab_feature): longint;                                                                                     cdecl; external DLL Name 'mitab_c_get_pen_width'           ;
function  mitab_c_get_projinfo           (dataset: mitab_handle): mitab_projinfo;                                                                               cdecl; external DLL Name 'mitab_c_get_projinfo'            ;
function  mitab_c_get_symbol_color       (feature: mitab_feature): longint;                                                                                     cdecl; external DLL Name 'mitab_c_get_symbol_color'        ;
function  mitab_c_get_symbol_no          (feature: mitab_feature): longint;                                                                                     cdecl; external DLL Name 'mitab_c_get_symbol_no'           ;
function  mitab_c_get_symbol_size        (feature: mitab_feature): longint;                                                                                     cdecl; external DLL Name 'mitab_c_get_symbol_size'         ;
function  mitab_c_get_text_vb            (feature: mitab_feature; text: pchar; l: longint): longint;                                                            cdecl; external DLL Name 'mitab_c_get_text_vb'             ;
function  mitab_c_get_text               (feature: mitab_feature): pchar;                                                                                       cdecl; external DLL Name 'mitab_c_get_text'                ;
function  mitab_c_get_text_angle         (feature: mitab_feature): double;                                                                                      cdecl; external DLL Name 'mitab_c_get_text_angle'          ;
function  mitab_c_get_text_bgcolor       (feature: mitab_feature): longint;                                                                                     cdecl; external DLL Name 'mitab_c_get_text_bgcolor'        ;
function  mitab_c_get_text_fgcolor       (feature: mitab_feature): longint;                                                                                     cdecl; external DLL Name 'mitab_c_get_text_fgcolor'        ;
function  mitab_c_get_text_height        (feature: mitab_feature): double;                                                                                      cdecl; external DLL Name 'mitab_c_get_text_height'         ;
function  mitab_c_get_text_justification (feature: mitab_feature): longint;                                                                                     cdecl; external DLL Name 'mitab_c_get_text_justification'  ;
function  mitab_c_get_text_linetype      (feature: mitab_feature): longint;                                                                                     cdecl; external DLL Name 'mitab_c_get_text_linetype'       ;
function  mitab_c_get_text_spacing       (feature: mitab_feature): longint;                                                                                     cdecl; external DLL Name 'mitab_c_get_text_spacing'        ;
function  mitab_c_get_text_width         (feature: mitab_feature): double;                                                                                      cdecl; external DLL Name 'mitab_c_get_text_width'          ;
function  mitab_c_get_type               (feature: mitab_feature): longint;                                                                                     cdecl; external DLL Name 'mitab_c_get_type'                ;
function  mitab_c_get_vertex_count       (feature: mitab_feature; part: longint): longint;                                                                      cdecl; external DLL Name 'mitab_c_get_vertex_count'        ;
function  mitab_c_get_vertex_x           (feature: mitab_feature; part, vertex: longint): double;                                                               cdecl; external DLL Name 'mitab_c_get_vertex_x'            ;
function  mitab_c_get_vertex_y           (feature: mitab_feature; part, vertex: longint): double;                                                               cdecl; external DLL Name 'mitab_c_get_vertex_y'            ;
function  mitab_c_getlasterrormsg_vb     (errormsg: pchar; l: longint): longint;                                                                                cdecl; external DLL Name 'mitab_c_getlasterrormsg_vb'      ;
function  mitab_c_getlasterrormsg        (): pchar;                                                                                                             cdecl; external DLL Name 'mitab_c_getlasterrormsg'         ;
function  mitab_c_getlasterrorno         (): longint;                                                                                                           cdecl; external DLL Name 'mitab_c_getlasterrorno'          ;
function  mitab_c_next_feature_id        (handle: mitab_handle; last_feature_id: longint): longint;                                                             cdecl; external DLL Name 'mitab_c_next_feature_id'         ;
function  mitab_c_open                   (filename: pchar): mitab_handle;                                                                                       cdecl; external DLL Name 'mitab_c_open'                    ;
function  mitab_c_read_feature           (handle:  mitab_handle; feature_id: longint): mitab_feature;                                                           cdecl; external DLL Name 'mitab_c_read_feature'            ;
procedure mitab_c_set_arc                (feature: mitab_feature; center_x, center_y, x_radius, y_radius, start_angle, end_angle: double);                      cdecl; external DLL Name 'mitab_c_set_arc'                 ;
procedure mitab_c_set_brush              (feature: mitab_feature; fg_color, bg_color, pattern, transparent: longint);                                           cdecl; external DLL Name 'mitab_c_set_brush'               ;
procedure mitab_c_set_field              (feature: mitab_feature; field_index: longint; value: pchar);                                                          cdecl; external DLL Name 'mitab_c_set_field'               ;
procedure mitab_c_set_font               (feature: mitab_feature; font_name: pchar);                                                                            cdecl; external DLL Name 'mitab_c_set_font'                ;
procedure mitab_c_set_pen                (feature: mitab_feature; width, pattern, color: longint);                                                              cdecl; external DLL Name 'mitab_c_set_pen'                 ;
procedure mitab_c_set_points             (feature: mitab_feature; part, vertex_count: longint; var x,y: double);                                                cdecl; external DLL Name 'mitab_c_set_points'              ;
function  mitab_c_set_projinfo           (dataset: mitab_handle; projinfo: mitab_projinfo): longint;                                                            cdecl; external DLL Name 'mitab_c_set_projinfo'            ;
procedure mitab_c_set_symbol             (feature: mitab_feature; symbol_no, symbol_size, symbol_color: longint);                                               cdecl; external DLL Name 'mitab_c_set_symbol'              ;
procedure mitab_c_set_text               (feature: mitab_feature; text: pchar);                                                                                 cdecl; external DLL Name 'mitab_c_set_text'                ;
procedure mitab_c_set_text_display       (feature: mitab_feature; angle, height, width: double; fg_color, bg_color, justification, spacing, linetype: longint); cdecl; external DLL Name 'mitab_c_set_text_display'        ;
function  mitab_c_write_feature          (handle: mitab_handle; feature: mitab_feature): longint;                                                               cdecl; external DLL Name 'mitab_c_write_feature'           ;

implementation
end.
