#pragma once

typedef struct
{
    // All the weights.
    float* w;
    // Hidden to output layer weights.
    float* x;
    // Biases.
    float* b;
    // Hidden layer.
    float* h;
    // Output layer.
    float* o;
    // Number of biases - always two - Tinn only supports a single hidden layer.
    int nb;
    // Number of weights.
    int nw;
    // Number of inputs.
    int nips;
    // Number of hidden neurons.
    int nhid;
    // Number of outputs.
    int nops;
}
Tinn;

float* xtpredict(Tinn,  float* in);

float xttrain(Tinn,  float* in,  float* tg, float rate);

Tinn xtbuild(int nips, int nhid, int nops);

void xtsave(Tinn,  char* path);

Tinn xtload( char* path);

void xtfree(Tinn);

void xtprint( float* arr,  int size);
