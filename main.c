#include <pocketsphinx.h>

int
main(int argc, char *argv[])
{
    char* filename = "error.log";
    err_set_logfile(filename);
    ps_decoder_t *ps;
    cmd_ln_t *config;
    FILE *fh;
    FILE *fh2;
    char const *hyp, *uttid;
    int16 buf[512];
    int rv;
    int32 score;

    config = cmd_ln_init(NULL, ps_args(), TRUE,
		         "-hmm", MODELDIR "/en-us/en-us",
		         "-lm", MODELDIR "/en-us/en-us.lm.bin",
	    		 "-dict", MODELDIR "/en-us/cmudict-en-us.dict",
		         NULL);
    if (config == NULL) {
	fprintf(stderr, "Failed to create config object, see log for details\n");
	return -1;
    }

    ps = ps_init(config);
    if (ps == NULL) {
	fprintf(stderr, "Failed to create recognizer, see log for details\n");
	return -1;
    }

    fh = fopen("goforward.raw", "rb");
    if (fh == NULL) {
	fprintf(stderr, "Unable to open input file goforward.raw\n");
	return -1;
    }
    fh2 = fopen("numbers.raw", "rb");
    if (fh == NULL) {
	fprintf(stderr, "Unable to open input file numbers.raw\n");
	return -1;
    }

    rv = ps_start_utt(ps);

    while (!feof(fh)) {
	size_t nsamp;
	nsamp = fread(buf, 2, 512, fh);
	rv = ps_process_raw(ps, buf, nsamp, FALSE, FALSE);
    }
       
    rv = ps_end_utt(ps);
    hyp = ps_get_hyp(ps, &score);
    printf("Recognized from goforward.raw: %s\n", hyp);
    
    rv = ps_start_utt(ps);
    while (!feof(fh2)) {
	size_t nsamp;
	nsamp = fread(buf, 2, 512, fh2);
	rv = ps_process_raw(ps, buf, nsamp, FALSE, FALSE);
    }
    
    rv = ps_end_utt(ps);
    hyp = ps_get_hyp(ps, &score);
    printf("Recognized from numbers.raw: %s\n", hyp);


    fclose(fh);
    fclose(fh2);
    ps_free(ps);
    cmd_ln_free_r(config);

    return 0;
}