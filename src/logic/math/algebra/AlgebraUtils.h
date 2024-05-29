//
// Created by faliszewskii on 30.12.23.
//

#ifndef OPENGL_SANDBOX_ALGEBRAUTILS_H
#define OPENGL_SANDBOX_ALGEBRAUTILS_H


#include <glm/gtc/quaternion.hpp>
#include <iostream>
#include <glm/gtx/io.hpp>
#include <vector>

class AlgebraUtils {
public:
    static glm::vec3 getFront(glm::quat q) {
        return glm::normalize(q * glm::vec3(0, 0, -1.0f));
    }

    static glm::vec3 getUp(glm::quat q) {
        return glm::normalize(q * glm::vec3(0, 1.0f, 0));
    }

    static glm::vec3 getRight(glm::quat q) {
        return glm::normalize(q * glm::vec3(1.0f, 0, 0));
    }

    static void decomposeMtx(const glm::mat4 &m, glm::vec3 &pos, glm::quat &rot, glm::vec3 &scale);

    template<typename T, typename U>
    static std::vector<T> solveTridiagonal(std::vector<U> a, std::vector<U> b, std::vector<U> c, std::vector<T> d) {
        int order = d.size();
        if(order == 1)
            return {d[0] / b[0]};

        std::vector<U> cPrim(order - 1);
        cPrim[0] = c[0]/b[0];
        for(int i=1; i<order-1; i++)
            cPrim[i] = c[i] / (b[i] - a[i-1]*cPrim[i-1]);

        std::vector<T> dPrim(order);
        dPrim[0] = d[0]/b[0];
        for(int i=1; i<order; i++)
            dPrim[i] = (d[i] - a[i-1] * dPrim[i-1]) / (b[i] - a[i-1] * cPrim[i-1]);

        std::vector<T> x(order);
        x[order-1] = dPrim[order-1];
        for(int i=order-2; i>=0; i--)
            x[i] = dPrim[i] - cPrim[i] * x[i+1];

        return x;
    }

};


#endif //OPENGL_SANDBOX_ALGEBRAUTILS_H
