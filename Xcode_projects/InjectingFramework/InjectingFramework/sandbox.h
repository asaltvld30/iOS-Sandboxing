//
//  sandbox.h
//  Sample
//
//  Created by Vlad Corneci on 31/03/2019.
//  Copyright © 2019 Vlad Corneci. All rights reserved.
//

#ifndef sandbox_h
#define sandbox_h

typedef struct {
    unsigned int type;
    void *blob;
    size_t len;
} sbProfile_t;

typedef struct {} sbParams_t;

extern sbParams_t *sandbox_create_params(void);
extern sbProfile_t *sandbox_compile_file(char *profile_file, sbParams_t *params, char **err);
extern sbProfile_t *sandbox_compile_named(char *profile_name, sbParams_t *params, char **err);
extern sbProfile_t *sandbox_compile_string(char *profile_string, sbParams_t *params, char **err);
extern int sandbox_apply_container(sbProfile_t *sandbox, int flags);
extern void sandbox_free_profile(sbProfile_t *compiled_profile);

#endif /* sandbox_h */
