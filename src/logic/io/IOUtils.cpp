//
// Created by faliszewskii on 28.12.23.
//

#include <string>
#include "IOUtils.h"

std::string IOUtils::getResource(std::string relativePath) {
    return std::string("../../resources/").append(relativePath);
}
