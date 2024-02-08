
void __cdecl N::N(N *this, int a2)
{
  *(_DWORD *)this = off_8048848;
  *((_DWORD *)this + 26) = a2;
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
  N *instance_1; // ebx
  N *instance_2; // ebx
  N *instance_3; // [esp+1Ch] [ebp-8h]

  if ( argc <= 1 )
    _exit(1);

  // init 1
  instance_1 = new N(12);
  N::N(instance_1, 5);

  instance_3 = instance_1; //meme ptr

  // init 2
  instance_2 = new N(12);
  N::N(instance_2, 6);

  N::setAnnotation(instance_3, argv[1]); //ecrit sur instance 2 | ecrire shellcode
  
  return (**(int (__cdecl ***)(N *, N *))instance_2)(instance_2, instance_3); //function ptr ???? jup sur instance 2
}

class N
{
	param1
	param2

	this->param1
	this + 4
}
