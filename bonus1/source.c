int __cdecl main(int argc, const char **argv, const char **envp)
{
  char dest[40]; // [esp+14h] [ebp-2Ch] BYREF
  int nb; // [esp+3Ch] [ebp-4h]

  nb = atoi(argv[1]);
  if ( nb > 9 )
    return 1;
  memcpy(dest, argv[2], 4 * nb);
  if ( nb == 1464814662 )
    execl("/bin/sh", "sh", 0);
  return 0;
}