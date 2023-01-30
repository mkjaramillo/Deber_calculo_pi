#include <iostream>
#include <omp.h>
#include<chrono>
#include <benchmark/benchmark.h>

namespace ch=std::chrono;
#define NUMERO_ITERACIONES 1000000000

//Metodo serial
double pi_serial(){
    double respuesta=0.0;

    for(long i=0; i <= NUMERO_ITERACIONES; i++) {
        if(i%2==0) {
            respuesta+=4.0/(2.0* i+1.0);
        }else {
            respuesta-=4.0/(2.0* i+1.0);
        }
    }
    return respuesta;
}

//Metodo con una seccion paralela
double pi_omp1(){
    int numero_hilos=0;
    #pragma omp parallel
        {
            //le pone en hilo cero
            #pragma opm master
            numero_hilos= omp_get_num_threads();
        };

    double sumas_parciales[numero_hilos];

    #pragma omp parallel shared(sumas_parciales)//para demostrar que esta variable es global
    {
        //variable local
        int thread_id= omp_get_thread_num();
        for(long i=thread_id; i <= NUMERO_ITERACIONES; i+=numero_hilos) {
            if(i%2==0) {
                sumas_parciales[thread_id]+=4.0/(2.0* i+1.0);
            }else {
                sumas_parciales[thread_id]-=4.0/(2.0* i+1.0);
            }
        }
    }
    double respuesta=0;
    //suma de todos los hilos
    for(int i=0;i<=numero_hilos;i++){
        respuesta=respuesta+sumas_parciales[i];
    }
    return respuesta;
}

//Metodo con for parallelo
double pi_omp_for_parallel() {
    int numero_hilos=0;
    #pragma omp parallel
        {
            #pragma opm master
            numero_hilos= omp_get_num_threads();
        };

    double sumas_parciales[numero_hilos];
    #pragma omp parallel shared(sumas_parciales)
        {
            //variable local
            int thread_id= omp_get_thread_num();
            //for paralelo
            #pragma omp for
            for(long i=0; i <= NUMERO_ITERACIONES; i++) {
                if(i%2==0) {
                    sumas_parciales[thread_id]+=4.0/(2.0* i+1.0);
                }else {
                    sumas_parciales[thread_id]-=4.0/(2.0* i+1.0);
                }
            }
        }
    double respuesta=0;

    for(int i=0;i<=numero_hilos;i++){
        respuesta=respuesta+sumas_parciales[i];
    }
    return respuesta;
}

//Defino las funciones para el benchmark
static void BM_omp_pi_Serial (benchmark::State& state) {
    double value;
    for (auto _ : state){
        value = pi_serial();
    }
    std::printf("El valor de pi Serial es %f\n", value);
}
static void BM_omp_seccion_parallel (benchmark::State& state) {
    double value;
    for (auto _ : state){
        value=pi_omp1();
    }
    std::printf("El valor de pi Seccion Parallel es %f\n", value);
}
static void BM_omp_for_parallel (benchmark::State& state) {
    double value;
    for (auto _ : state){
        value=pi_omp_for_parallel();
    }
    std::printf("El valor de pi For Parallel es %f\n", value);
}
//Registro las funciones en un benchmark

BENCHMARK(BM_omp_pi_Serial)
        ->Unit(benchmark::kMillisecond)
        ->Iterations(1)
        ->Repetitions(1);
BENCHMARK(BM_omp_seccion_parallel)
        ->Unit(benchmark::kMillisecond)
        ->Iterations(1)
        ->Repetitions(1);

BENCHMARK(BM_omp_for_parallel)
        ->Unit(benchmark::kMillisecond)
        ->Iterations(1)
        ->Repetitions(1);
BENCHMARK_MAIN();



