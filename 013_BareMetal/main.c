#include<stdint.h>
#include<stdio.h>

uint32_t global_count = 0U;


#ifdef ENABLE_C_LIB
extern void initialise_monitor_handles();
extern void init_all_fault_handlers();
#endif 

int main()
{
#ifdef ENABLE_C_LIB
  init_all_fault_handlers();
#endif
  //	initialise_monitor_handles();
  // const char msg[] = "Hello, world!\n";
  //  write(0, msg, sizeof(msg) - 1);  // Write message to stdout (file descriptor 0)

  const uint32_t x = 1600000U;

  for (;;)
  {

    for (uint32_t i = 0; i < x; i++)
      ;
  #ifdef ENABLE_C_LIB
    printf("global count == %ld\n", global_count);
  #endif
    global_count++;
  }

  return 0;
}
