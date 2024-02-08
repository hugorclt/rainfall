
void __cdecl N::N(N *this, int nb)
{
  *(_DWORD *)this = off_8048848;
  *((_DWORD *)this + 26) = nb;
}

void *__cdecl N::setAnnotation(N *this, char *s)
{
  size_t len_input; // eax

  len_input = strlen(s);
  return memcpy((char *)this + 4, s, len_input);
}

int __cdecl N::operator+(int a1, int a2)
{
  return *(_DWORD *)(a1 + 104) + *(_DWORD *)(a2 + 104);
}

int __cdecl N::operator-(int a1, int a2)
{
  return *(_DWORD *)(a1 + 104) - *(_DWORD *)(a2 + 104);
}


int __cdecl main(int argc, const char **argv, const char **envp)
{
  N *instance_1;
  N *instance_2;

  if ( argc <= 1 )
    _exit(1);

  // init 1
  instance_1 = new N(12);
  N::N(instance_1, 5);


  // init 2
  instance_2 = new N(12);
  N::N(instance_2, 6);

  N::setAnnotation(instance_1, argv[1]); //ecrit sur instance 2 | ecrire shellcode
  
  // return (**(int (__cdecl ***)(N *, N *))instance_2)(instance_2, instance_1); //function ptr ????

  //ghidra version
  return (**(code **)*instance_2)(instance_2, instance_1);
}

void main(int param_1,int param_2)

// {
//   N *this;
//   undefined4 *this_00;
  
//   if (param_1 < 2) {
//                     // WARNING: Subroutine does not return
//     _exit(1);
//   }
//   this = (N *)operator_new(0x6c);
//   N::N(this,5);
//   this_00 = (undefined4 *)operator_new(0x6c);
//   N::N((N *)this_00,6);
//   N::setAnnotation(this,*(char **)(param_2 + 4));
//   (**(code **)*this_00)(this_00,this);
//   return;
// }
