/*
 *      confparse/confparse.c
 *      Copyright © 2008,2009 Martin Duquesnoy <xorg62@gmail.com>
 *      All rights reserved.
 *
 *      Redistribution and use in source and binary forms, with or without
 *      modification, are permitted provided that the following conditions are
 *      met:
 *
 *      * Redistributions of source code must retain the above copyright
 *        notice, this list of conditions and the following disclaimer.
 *      * Redistributions in binary form must reproduce the above
 *        copyright notice, this list of conditions and the following disclaimer
 *        in the documentation and/or other materials provided with the
 *        distribution.
 *      * Neither the name of the  nor the names of its
 *        contributors may be used to endorse or promote products derived from
 *        this software without specific prior written permission.
 *
 *      THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
 *      "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
 *      LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
 *      A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT
 *      OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
 *      SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
 *      LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
 *      DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
 *      THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 *      (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
 *      OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */

#include "confparse.h"

char*
file_to_str(FILE *f)
{
     char c;
     char *ret;
     int i;

     for(i = 1; fgetc(f) != EOF; ++i);

     ret = emalloc(i, sizeof(char));

     rewind(f);

     for(i = 0; (c = fgetc(f)) != EOF; ret[i++] = c);

     fclose(f);

     return ret;
}

char*
get_sec(char *src, char *name)
{
     char *ret, *p;
     char *secn;

     if(!src)
          return NULL;

     if(!name)
          return src;

     secn = emalloc((strlen(name) + 2), sizeof(char));
     sprintf(secn, "%c%s%c", SEC_DEL_S, name, SEC_DEL_E);

     ret = _strdup(src);

     if((p = strstr(erase_delim_content(src), secn)))
     {
          ret += strlen(ret) - strlen(p) + strlen(secn) + 1;

          if((p = strstr(erase_delim_content(ret), secn)))
               *(ret + (strlen(ret) - strlen(p))) = '\0';
          else
               ret = NULL;
     }
     else
          ret = NULL;

     free(secn);

     return ret;
}

char*
get_nsec(char *src, char *name, int n)
{
     int i;
     char *ret, *buf, *secn;
     char *buf2;

     if(!src)
          return NULL;

     if(!name)
          return src;

     if(!n)
          return get_sec(src, name);

     secn = emalloc((strlen(name) + 2), sizeof(char));
     sprintf(secn, "%c%s%c", SEC_DEL_S, name, SEC_DEL_E);

     buf = erase_delim_content(src);
     buf2 = erase_sec_content(buf);

     for(i = 0; i < n * 2 && (buf = strstr(buf, secn)); ++i, buf += strlen(secn));

     ret = get_sec(src + strlen(src) - strlen(buf), name);

     free(secn);

     return ret;
}

int
get_size_sec(char *src, char *name)
{
     int ret;
     char *secn, *buf;

     if(!src || !name)
          return 0;

     secn = emalloc(strlen(name), sizeof(char));
     sprintf(secn, "%c%s%c", SEC_DEL_S, name, SEC_DEL_E);

     buf = erase_sec_content(src);

     for(ret = 0; (buf = strstr(buf, secn)); ++ret, buf += strlen(secn));

     /* If a section is not closed */
     for(; ret % 2; --ret);

     free(secn);

     return ret / 2;
}

opt_type
get_opt(char *src, char *def, char *name)
{
     int i;
     char *p = NULL;
     opt_type ret = null_opt_type;

     if(!src || !name)
          return (def) ? str_to_opt(def) : null_opt_type;

     if((p = opt_srch(erase_sec_content(src), name)))
     {
          for(i = 0; p[i] && p[i] != '\n'; ++i);
          p[i] = '\0';

          if((p = strchr(p, '=')) && !is_in_delimiter(p, 0))
               ret = str_to_opt(clean_value(++p));
     }
     else
          ret = str_to_opt(def);

     return ret;
}

/* option = {val1, val2, val3} */
opt_type*
get_list_opt(char *src, char *def, char *name, int *n)
{
     int i, j;
     char *p, *p2;
     opt_type *ret;

     if(!src || !name)
          return NULL;

     *n = 0;

     if(!(p = get_opt(src, def, name).str))
          return NULL;

     for(i = 0; p[i] && (p[i] != '}' || is_in_delimiter(p, i)); ++i);
     p[i + 1] = '\0';

     /* Syntax of list (val1, val2, ..., valx) */
     if(*p != '{' || *(p + strlen(p) - 1) != '}')
          return NULL;

     /* Erase ( ) */
     ++p;
     *(p + strlen(p) - 1) = '\0';

     /* > 1 value in list */
     if(strchr(p, ','))
     {
          /* Count ',' */
          for(i = 0, *n = 1; i < strlen(p); ++i)
               if(p[i] == ',' && !is_in_delimiter(p, i))
                    ++(*n);

          ret = emalloc(*n, sizeof(opt_type));

          p2 = _strdup(p);

          /* Set all value in return array */
          for(i = j = 0; i < *n; ++i, p2 += ++j + 1)
          {
               for(j = 0; j < strlen(p2) && (p2[j] != ',' || is_in_delimiter(p2, j)); ++j);
               p2[j] = '\0';

               ret[i] = str_to_opt(clean_value(p2));
          }
     }
     else
     {
          ret = emalloc(1, sizeof(opt_type));
          *ret = str_to_opt(clean_value(p));

          *n = 1;
     }

     return ret;
}