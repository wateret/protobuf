// Protocol Buffers - Google's data interchange format
// Copyright 2008 Google Inc.  All rights reserved.
// https://developers.google.com/protocol-buffers/
//
// Redistribution and use in source and binary forms, with or without
// modification, are permitted provided that the following conditions are
// met:
//
//     * Redistributions of source code must retain the above copyright
// notice, this list of conditions and the following disclaimer.
//     * Redistributions in binary form must reproduce the above
// copyright notice, this list of conditions and the following disclaimer
// in the documentation and/or other materials provided with the
// distribution.
//     * Neither the name of Google Inc. nor the names of its
// contributors may be used to endorse or promote products derived from
// this software without specific prior written permission.
//
// THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
// "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
// LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
// A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT
// OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
// SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
// LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
// DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
// THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
// (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
// OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.

#import "GPBRootObject.h"

@class GPBDescriptor;
@class GPBCodedInputStream;
@class GPBCodedOutputStream;
@class GPBExtensionField;
@class GPBFieldDescriptor;
@class GPBUnknownFieldSet;

// In DEBUG ONLY, an NSException is thrown when a parsed message doesn't
// contain required fields. This key allows you to retrieve the parsed message
// from the exception's |userInfo| dictionary.
#ifdef DEBUG
extern NSString *const GPBExceptionMessageKey;
#endif  // DEBUG

// NOTE:
// If you add a instance method/property to this class that may conflict with
// methods declared in protos, you need to update objective_helpers.cc.
// The main cases are methods that take no arguments, or setFoo:/hasFoo: type
// methods.
@interface GPBMessage : GPBRootObject<NSCoding, NSCopying>

@property(nonatomic, readonly) GPBUnknownFieldSet *unknownFields;

// Are all required fields in the message and all embedded messages set.
@property(nonatomic, readonly, getter=isInitialized) BOOL initialized;

// Returns an autoreleased instance.
+ (instancetype)message;

// Create a message based on a variety of inputs.
// In DEBUG ONLY
// @throws NSInternalInconsistencyException The message is missing one or more
//         required fields (i.e. -[isInitialized] returns false). Use
//         GGPBExceptionMessageKey to retrieve the message from |userInfo|.
+ (instancetype)parseFromData:(NSData *)data;
+ (instancetype)parseFromData:(NSData *)data
            extensionRegistry:(GPBExtensionRegistry *)extensionRegistry;
+ (instancetype)parseFromCodedInputStream:(GPBCodedInputStream *)input
                        extensionRegistry:
                            (GPBExtensionRegistry *)extensionRegistry;

// Create a message based on delimited input.
+ (instancetype)parseDelimitedFromCodedInputStream:(GPBCodedInputStream *)input
                                 extensionRegistry:
                                     (GPBExtensionRegistry *)extensionRegistry;

- (instancetype)initWithData:(NSData *)data;
- (instancetype)initWithData:(NSData *)data
           extensionRegistry:(GPBExtensionRegistry *)extensionRegistry;
- (instancetype)initWithCodedInputStream:(GPBCodedInputStream *)input
                       extensionRegistry:
                           (GPBExtensionRegistry *)extensionRegistry;

// Serializes the message and writes it to output.
- (void)writeToCodedOutputStream:(GPBCodedOutputStream *)output;
- (void)writeToOutputStream:(NSOutputStream *)output;

// Serializes the message and writes it to output, but writes the size of the
// message as a variant before writing the message.
- (void)writeDelimitedToCodedOutputStream:(GPBCodedOutputStream *)output;
- (void)writeDelimitedToOutputStream:(NSOutputStream *)output;

// Serializes the message to an NSData. Note that this value is not cached, so
// if you are using it repeatedly, cache it yourself.
// In DEBUG ONLY:
// @throws NSInternalInconsistencyException The message is missing one or more
//         required fields (i.e. -[isInitialized] returns false). Use
//         GPBExceptionMessageKey to retrieve the message from |userInfo|.
- (NSData *)data;

// Same as -[data], except a delimiter is added to the start of the data
// indicating the size of the message data that follows.
- (NSData *)delimitedData;

// Returns the size of the object if it were serialized.
// This is not a cached value. If you are following a pattern like this:
// size_t size = [aMsg serializedSize];
// NSMutableData *foo = [NSMutableData dataWithCapacity:size + sizeof(size)];
// [foo writeSize:size];
// [foo appendData:[aMsg data]];
// you would be better doing:
// NSData *data = [aMsg data];
// NSUInteger size = [aMsg length];
// NSMutableData *foo = [NSMutableData dataWithCapacity:size + sizeof(size)];
// [foo writeSize:size];
// [foo appendData:data];
- (size_t)serializedSize;

// Return the descriptor for the message
+ (GPBDescriptor *)descriptor;
- (GPBDescriptor *)descriptor;

// Extensions use boxed values (NSNumbers) for PODs, NSMutableArrays for
// repeated. If the extension is a Message, just like fields, one will be
// auto created for you and returned.
- (BOOL)hasExtension:(GPBExtensionField *)extension;
- (id)getExtension:(GPBExtensionField *)extension;
- (void)setExtension:(GPBExtensionField *)extension value:(id)value;
- (void)addExtension:(GPBExtensionField *)extension value:(id)value;
- (void)setExtension:(GPBExtensionField *)extension
               index:(NSUInteger)index
               value:(id)value;
- (void)clearExtension:(GPBExtensionField *)extension;

- (void)setUnknownFields:(GPBUnknownFieldSet *)unknownFields;

// Resets all fields to their default values.
- (void)clear;

// Parses a message of this type from the input and merges it with this
// message.
- (void)mergeFromData:(NSData *)data
    extensionRegistry:(GPBExtensionRegistry *)extensionRegistry;

// Merges the fields from another message (of the same type) into this
// message.
- (void)mergeFrom:(GPBMessage *)other;

@end
