//  Injection.m
//  InjectingFramework
//
//  Created by Vlad Corneci on 29/12/2020.
//

#import <Foundation/Foundation.h>
#import <UIKit/UIKit.h>
#include <stdio.h>
#include <stdlib.h>

#include "my_sandbox.h"

__attribute__((constructor))
void EntryPoint() {
    NSLog(@"Hello from injectedd code");
    printf("Hello from injectedd code.\n");

//    NSFileManager *fileManager = [NSFileManager defaultManager];
    NSURL *bundleURL = [[NSBundle mainBundle] bundleURL];
//    NSArray *contents = [fileManager contentsOfDirectoryAtURL:bundleURL
//                                   includingPropertiesForKeys:@[]
//                                                      options:NSDirectoryEnumerationSkipsHiddenFiles
//                                                        error:nil];

    NSLog(@"Bunlde URL: %@", [bundleURL path]);
//    for (NSString *path in contents) {
//        NSLog(@"Content: %@", path);
//    }

    NSString* sandboxPath = [[bundleURL path] stringByAppendingString:@"/sandbox.bin"];
    NSLog(@"Sandbxox path: %@", sandboxPath);

    apply_sandbox_profile([sandboxPath cStringUsingEncoding:NSASCIIStringEncoding]);
}
