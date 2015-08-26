// Copyright 2013-present Facebook. All Rights Reserved.

#include <pbxbuild/FileTypeResolver.h>
#include <fstream>

using pbxbuild::FileTypeResolver;
using libutil::FSUtil;
using libutil::Wildcard;

pbxspec::PBX::FileType::shared_ptr FileTypeResolver::
Resolve(pbxspec::Manager::shared_ptr const &specManager, std::string const &filePath)
{
    bool isFolder = FSUtil::TestForDirectory(filePath);
    bool isReadable = FSUtil::TestForRead(filePath);

    std::string fileExtension = FSUtil::GetFileExtension(filePath);
    std::string fileName = FSUtil::GetBaseName(filePath);

    std::ifstream fileHandle;
    std::vector<uint8_t> fileContents;

    for (pbxspec::PBX::FileType::shared_ptr const &fileType : specManager->fileTypes()) {
        if (fileType->isFolder() != isFolder) {
            continue;
        }

        for (std::string const &pattern : fileType->filenamePatterns()) {
            if (Wildcard::Match(pattern, fileName)) {
                return fileType;
            }
        }

        if (isReadable) {
            for (std::vector<uint8_t> const &magicWord : fileType->magicWords()) {
                if (fileContents.size() < magicWord.size()) {
                    if (!fileHandle.is_open()) {
                        fileHandle.open(fileName, std::ios::in | std::ios::binary);
                        if (!fileHandle.good()) {
                            continue;
                        }
                    }

                    size_t offset = fileContents.size();
                    fileContents.resize(magicWord.size());
                    fileHandle.read((char *)&fileContents[offset], magicWord.size() - offset);
                    if (!fileHandle.good()) {
                        fileContents.resize(offset);
                        continue;
                    }
                }

                if (std::equal(magicWord.begin(), magicWord.end(), fileContents.begin())) {
                    return fileType;
                }
            }
        }

        for (std::string const &extension : fileType->extensions()) {
            if (extension == fileExtension) {
                return fileType;
            }
        }
    }

    return nullptr;
}

pbxspec::PBX::FileType::shared_ptr FileTypeResolver::
Resolve(pbxspec::Manager::shared_ptr const &specManager, pbxproj::PBX::FileReference::shared_ptr const &fileReference, pbxsetting::Environment const &environment)
{
    if (!fileReference->explicitFileType().empty()) {
        if (pbxspec::PBX::FileType::shared_ptr const &fileType = specManager->fileType(fileReference->explicitFileType())) {
            return fileType;
        }
    }

    if (!fileReference->lastKnownFileType().empty()) {
        if (pbxspec::PBX::FileType::shared_ptr const &fileType = specManager->fileType(fileReference->lastKnownFileType())) {
            return fileType;
        }
    }

    return Resolve(specManager, environment.expand(fileReference->resolve()));
}
