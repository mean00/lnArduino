

extern "C" void mutex_init(void *mtx)
{


}extern "C" void recursive_mutex_init(void *mtx)
{


}
extern "C" void spin_locks_reset(void)
{

}

extern "C" void watchdog_start_tick(unsigned int cycles) 
{

}

extern "C" bool running_on_fpga()
{
  return false;
}
extern "C" int stderr=0;
