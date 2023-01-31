#ifndef STDATOMIC_H
#define STDATOMIC_H

#define ATOMIC_FLAG_INIT	{ 0 }

typedef enum
{
    memory_order_relaxed = __ATOMIC_RELAXED,
    memory_order_consume = __ATOMIC_CONSUME,
    memory_order_acquire = __ATOMIC_ACQUIRE,
    memory_order_release = __ATOMIC_RELEASE,
    memory_order_acq_rel = __ATOMIC_ACQ_REL,
    memory_order_seq_cst = __ATOMIC_SEQ_CST
} memory_order;

typedef struct __atomic_flag_base
{
  bool _M_i;
} __atomic_flag_base;

typedef __atomic_flag_base atomic_flag;


bool atomic_flag_test_and_set_explicit(volatile __atomic_flag_base*, memory_order);
inline bool atomic_flag_test_and_set(volatile __atomic_flag_base* __a){ 
    return atomic_flag_test_and_set_explicit(__a, memory_order_seq_cst); 
}

void atomic_flag_clear_explicit(volatile __atomic_flag_base*, memory_order);
inline void atomic_flag_clear(volatile __atomic_flag_base* __a){ 
    atomic_flag_clear_explicit(__a, memory_order_seq_cst); 
}


#endif