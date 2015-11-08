/**
 Copyright (c) 2015-present, Facebook, Inc.
 All rights reserved.

 This source code is licensed under the BSD-style license found in the
 LICENSE file in the root directory of this source tree. An additional grant
 of patent rights can be found in the PATENTS file in the same directory.
 */

#ifndef __builtin_productPackagingUtility_Options_h
#define __builtin_productPackagingUtility_Options_h

#include <builtin/Base.h>

namespace builtin {
namespace productPackagingUtility {

class Options {
private:
    std::string _input;
    std::string _output;

private:
    bool        _removeFile;
    bool        _entitlements;
    bool        _resourceRules;

private:
    std::string _format;

public:
    Options();
    ~Options();

public:
    std::string const &input() const
    { return _input; }
    std::string const &output() const
    { return _output; }

private:
    bool removeFile() const
    { return _removeFile; }
    bool entitlements() const
    { return _entitlements; }

public:
    std::string const &format() const
    { return _format; }

private:
    friend class libutil::Options;
    std::pair<bool, std::string>
    parseArgument(std::vector<std::string> const &args, std::vector<std::string>::const_iterator *it);
};

}
}

#endif // !__builtin_productPackagingUtility_Options_h
