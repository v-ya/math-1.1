#include "wav.h"

vya_wav* wav_new(double time);
vya_wav* wav_load(char **path);
void wav_free(vya_wav *wav);
int wav_write(vya_wav *wav, char *path);
s32 loudness(double f);
double loudexpe(s32 value);
void clear(vya_wav *wav, u32 b, s32 t);
void rsin_inc(vya_wav *wav, u32 n, s32 loud, u32 b, s32 t);
void rcos_inc(vya_wav *wav, u32 n, s32 loud, u32 b, s32 t);
s32 rsin_get(vya_wav *wav, u32 n, u32 b, s32 t);
s32 rcos_get(vya_wav *wav, u32 n, u32 b, s32 t);

double noise(double *sa, u32 n);

