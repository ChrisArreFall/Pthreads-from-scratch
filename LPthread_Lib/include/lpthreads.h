#ifndef THREAD_H
#define THREAD_H

#include <stdio.h>



void initLpthreads();
int Lthread_create(void (*func)(void), int scheduler);
void Lthread_start(void (*func)(void));
int Lthread_end();
void Lthread_yield();
int Lthread_join(int id);
void Lthread_detach();
void Lthread_wait();
void Lthread_yieldSp();

void obtainNext();

void Lmutex_init();
void Lmutex_destroy();
void Lmutex_unlock();
void Lmutex_trylock();
#endif