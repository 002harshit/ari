#include <stdio.h>

int
main()
{
	FILE *c_file = fopen("keymap.h", "w");
	char *file_headers = "#ifndef ARI_KEYMAP_H\n"
						 "#define ARI_KEYMAP_H\n\n"
						 "#define DEFAULT_WAV \"wav/default\"\n"
						 "static const char *wav_filelist[] = {";
	char *file_footers = "\n};\n#endif //// ARI_KEYMAP_H";

	fprintf(c_file, "%s", file_headers);
	for (int i = 0; i < 256; i++) {
		for (int j = 0; j < 2; j++) {
			char wav_filename[32];
			sprintf(wav_filename, "wav/%02x-%1d.wav", i, j);
			FILE *f = fopen(wav_filename, "r");
			if (!f) {
				fprintf(c_file, "\n\t\"%s-%1d.wav\",\t\t// %3d", "wav/default", j, i);
			} else {
				fprintf(c_file, "\n\t\"%s\",\t\t\t// %3d", wav_filename, i);
			}
		}
	}
	fprintf(c_file, "%s", file_footers);
	fclose(c_file);
	return 0;
}
