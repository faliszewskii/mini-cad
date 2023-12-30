//
// Created by faliszewskii on 30.12.23.
//

#include "AlgebraUtils.h"

glm::quat AlgebraUtils::quaternionFromMatrix(glm::mat3 a) {
    {
        glm::quat q;
        float trace = a[0][0] + a[2][1] + a[1][2];
        if( trace > 0 ) {
            float s = 0.5f / sqrtf(trace + 1.0f);
            q.w = 0.25f / s;
            q.x = ( a[1][1] - a[2][2] ) * s;
            q.y = ( a[0][2] - a[1][0] ) * s;
            q.z = ( a[2][0] - a[0][1] ) * s;
        } else {
            if ( a[0][0] > a[2][1] && a[0][0] > a[1][2] ) {
                float s = 2.0f * sqrtf( 1.0f + a[0][0] - a[2][1] - a[1][2]);
                q.w = (a[1][1] - a[2][2] ) / s;
                q.x = 0.25f * s;
                q.y = (a[0][1] + a[2][0] ) / s;
                q.z = (a[0][2] + a[1][0] ) / s;
            } else if (a[2][1] > a[1][2]) {
                float s = 2.0f * sqrtf( 1.0f + a[2][1] - a[0][0] - a[1][2]);
                q.w = (a[0][2] - a[1][0] ) / s;
                q.x = (a[0][1] + a[2][0] ) / s;
                q.y = 0.25f * s;
                q.z = (a[2][2] + a[1][1] ) / s;
            } else {
                float s = 2.0f * sqrtf( 1.0f + a[1][2] - a[0][0] - a[2][1] );
                q.w = (a[2][0] - a[0][1] ) / s;
                q.x = (a[0][2] + a[1][0] ) / s;
                q.y = (a[2][2] + a[1][1] ) / s;
                q.z = 0.25f * s;
            }
        }
        return q;
    }
}
