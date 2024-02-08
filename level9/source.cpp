
void __cdecl N::N(N *this, int a2)
{
  *(_DWORD *)this = off_8048848;
  *((_DWORD *)this + 26) = a2;
}

void *__cdecl N::setAnnotation(N *this, char *s)
{
  size_t v2; // eax

  v2 = strlen(s);
  return memcpy((char *)this + 4, s, v2);
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
  N *v3; // ebx
  N *v4; // ebx
  N *v6; // [esp+1Ch] [ebp-8h]

  if ( argc <= 1 )
    _exit(1);

  v3 = new N(12);
  N::N(v3, 5);
  v6 = v3;

  v4 = new N(12);
  N::N(v4, 6);

  N::setAnnotation(v6, argv[1]);
  
  return (**(int (__cdecl ***)(N *, N *))v4)(v4, v6);
}