#include <stdint.h>
#include <vx_intrinsics.h>
#include <vx_spawn.h>
#include "common.h"

void kernel_body(int task_id, void* arg) {
	struct kernel_arg_t* _arg = (struct kernel_arg_t*)(arg);
	uint32_t* src_ptr = (uint32_t*)_arg->src_ptr;
	uint32_t* dst_ptr = (uint32_t*)_arg->dst_ptr;

	int32_t* addr_ptr = (int32_t*)(src_ptr[task_id]);

	dst_ptr[task_id] = *addr_ptr;
}

void main() {
	struct kernel_arg_t* arg = (struct kernel_arg_t*)KERNEL_ARG_DEV_MEM_ADDR;
	vx_spawn_tasks(arg->num_points, kernel_body, arg);
}