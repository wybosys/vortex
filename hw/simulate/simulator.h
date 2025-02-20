#pragma once

#include <verilated.h>

#ifdef AXI_BUS
#include "VVortex_axi.h"
#include "VVortex_axi__Syms.h"
#else
#include "VVortex.h"
#include "VVortex__Syms.h"
#endif

#ifdef VCD_OUTPUT
#include <verilated_vcd_c.h>
#endif

#include <VX_config.h>
#include "ram.h"

#include <ostream>
#include <list>
#include <vector>
#include <sstream> 
#include <unordered_map>

#ifndef MEMORY_BANKS
  #ifdef PLATFORM_PARAM_LOCAL_MEMORY_BANKS
    #define MEMORY_BANKS PLATFORM_PARAM_LOCAL_MEMORY_BANKS
  #else
    #define MEMORY_BANKS 2
  #endif
#endif

class Simulator {
public:
  
  Simulator();
  virtual ~Simulator();

  void attach_ram(RAM* ram);

  void load_bin(const char* program_file);
  void load_ihex(const char* program_file);
  
  bool is_busy() const;

  void reset();
  void step();
  void wait(uint32_t cycles);

  int run();

  void print_stats(std::ostream& out);

private:  

  typedef struct {
    int cycles_left;  
    std::array<uint8_t, MEM_BLOCK_SIZE> block;
    uint64_t addr;
    uint64_t tag;
  } mem_req_t;

  std::unordered_map<int, std::stringstream> print_bufs_;

  void eval();  
  
#ifdef AXI_BUS
  void reset_axi_bus();  
  void eval_axi_bus(bool clk); 
#else
  void reset_mem_bus();  
  void eval_mem_bus(bool clk);
#endif

  int get_last_wb_value(int reg) const;  
  
  bool get_ebreak() const;

  std::list<mem_req_t> mem_rsp_vec_ [MEMORY_BANKS];
  uint32_t last_mem_rsp_bank_;

  bool mem_rsp_active_;

  bool mem_rsp_ready_;

  RAM *ram_;

#ifdef AXI_BUS
  VVortex_axi *vortex_;
#else
  VVortex *vortex_;
#endif

#ifdef VCD_OUTPUT
  VerilatedVcdC *trace_;
#endif
};