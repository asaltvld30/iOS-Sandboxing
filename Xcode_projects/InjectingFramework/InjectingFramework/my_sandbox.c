//
//  my_sandbox.c
//  Sample
//
//  Created by Vlad Corneci on 31/03/2019.
//  Copyright © 2019 Vlad Corneci. All rights reserved.
//
#include <stdlib.h>
#include <sandbox.h>
#include <string.h>
#include "my_sandbox.h"
#include "sandbox.h"

#include <dlfcn.h>
#include <sys/types.h>
#include <unistd.h>

char *profileString = "(version 1) (allow default) (deny file-read-data (literal \"/private/var/tmp/can_w\")) (deny file-write-data (literal \"/private/var/tmp/can_r\"))";

static void dump_sandbox_profile(sbProfile_t *profile, char *path)
{
    FILE* file;

    fprintf(stderr, "Dumped compiled profile to %s\n", path);
    
    /* Open file */
    file = fopen(path, "wb");
    if (file == NULL) {
        fprintf(stderr, "Can't open profile.");
        exit(2);
    }

    /* Write profile type */
    fwrite(&profile->type, sizeof(unsigned int), 1, file);
    /* Write profile size */
    fwrite(&profile->len, sizeof(size_t), 1, file);
    /* Read profile blob */
    fwrite(profile->blob, profile->len, 1, file);

    /* Close file */
    fclose(file);

    fprintf(stderr, "Profile dumped\n");
}

static sbProfile_t *restore_sandbox_profile(const char *path)
{
    FILE *fp;
    size_t length;
    sbProfile_t *profile = calloc(1, sizeof(sbProfile_t));

    fprintf(stderr, "Restore compiled profile from %s\n", path);
    fp = fopen(path, "rb");
    if (fp == NULL) {
        fprintf(stderr, "Can't open profile.\n");
        exit(2);
    }

    /* Read profile type */
    fread(&profile->type, sizeof(unsigned int), 1, fp);

    /* Read profile size */
    fread(&length, sizeof(size_t), 1, fp);
    profile->len = length;

    /* Read profile blob */
    profile->blob = calloc(length, 1);
    fread(profile->blob, length, 1, fp);

    fclose(fp);

    return profile;
}

int apply_sandbox_profile(const char *path)
{
    int rc;
    sbProfile_t *compiled_profile = NULL;
//    (sbProfile_t *)calloc(1, sizeof(sbProfile_t));
    
    fprintf(stderr, "Compiled profile address: %p\n", compiled_profile);

    
//    fprintf(stderr, "Binary selected: %s\n", profile_name);
//    FILE *fp = fopen(profile_name, "r");
//    if (fp == NULL) {
//        perror("FP == null");
//    }
    
    // compiled_profile
//    fseek(fp, 0L, SEEK_END);
//    size_t ret_size, file_size = ftell(fp);
//    // rewind(fp);fseek(fptr, 0, SEEK_SET);
//    // fseek(fp, 0, SEEK_SET);
//    rewind(fp);
//
//    fprintf(stderr, "Binary size: %zu\n", file_size);
//    compiled_profile->blob = malloc(file_size);
//
//    ret_size = fread(compiled_profile->blob, 1, file_size, fp);
//    fprintf(stderr, "Finished reading %zu bytes\n", ret_size);
//    if (ret_size != file_size)
//        fprintf(stderr, "Binary file size differs");
    
//    compiled_profile->name = strdup("out.bin");
//    compiled_profile->len = profile_size;
//    compiled_profile->blob = malloc(profile_size);
//    memcpy(compiled_profile->blob, profileBinary, profile_size);
 
//    char *err;
//    sbParams_t *params = sandbox_create_params();
//    compiled_profile = sandbox_compile_string(profileString, params, &err);
    
//

//    dump_sandbox_profile(compiled_profile, "/tmp/profile.bin");
    compiled_profile = restore_sandbox_profile(path);

    if (!compiled_profile) {
        fprintf(stderr, "No compiled profile.\n");
        exit(2);
    }

    fprintf(stderr, "Profile name: %u\nProfile size: %zu\n",
        compiled_profile->type,
        compiled_profile->len
    );

    fprintf(stderr, "Applying container\n");
    rc = sandbox_apply_container(compiled_profile, 0x0);
    
    if (rc != 0) {
        perror("sandbox_apply_container");
    }

    if (compiled_profile) {
        fprintf(stderr, "Free\n");
        sandbox_free_profile(compiled_profile);
    }

    return 0;
}
