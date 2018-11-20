#ifndef	_file_h_
#define	_file_h_

#ifndef __nosym__

char* get_path(char *path_list, char *path_short);
char* load_string(char *path);
var* store_data(char *path, var *obj, u32 isfast);
var* load_data(var *obj, char *path);

#endif

#endif

