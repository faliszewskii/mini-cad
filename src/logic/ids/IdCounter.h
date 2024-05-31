//
// Created by faliszewskii on 03.03.24.
//

#ifndef OPENGL_SANDBOX_IDCOUNTER_H
#define OPENGL_SANDBOX_IDCOUNTER_H

class IdCounter {
    int counter = 0;
public:
    static int nextId() {
        static IdCounter idCounter;
        return idCounter.counter++;
    };

    static void reset() {}
};

#endif //OPENGL_SANDBOX_IDCOUNTER_H
