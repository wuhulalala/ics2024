#include <proc.h>
#include <elf.h>
#include <fs.h>
#include <nano.h>

#ifdef __LP64__
# define Elf_Ehdr Elf64_Ehdr
# define Elf_Phdr Elf64_Phdr
#else
# define Elf_Ehdr Elf32_Ehdr
# define Elf_Phdr Elf32_Phdr
#endif


static uintptr_t loader(PCB *pcb, const char *filename) {
  Elf_Ehdr elf;
  size_t offset = 0;
  int fd = fs_open(filename, 0, 0);

  assert(fs_read(fd, &elf, sizeof(Elf_Ehdr)) == sizeof(Elf_Ehdr));
  assert(*(uint32_t *)elf.e_ident == 0x464C457F);
  uint8_t buf[128];

  uintptr_t max_end = 0; 
  
  offset = elf.e_phoff;
  fs_lseek(fd, elf.e_phoff, SEEK_SET);

  size_t en_nr = elf.e_phentsize;

  Elf32_Phdr phdr;
  for (int i = 0; i < en_nr; i++) {
    assert(elf.e_phentsize == fs_read(fd, &phdr, sizeof(Elf32_Phdr)));
    if (phdr.p_type == PT_LOAD) {
      size_t nr = phdr.p_filesz;
      uintptr_t off = phdr.p_offset;
      fs_lseek(fd, off, SEEK_SET);
      uintptr_t va = phdr.p_vaddr;
      size_t len = nr > 128 ? 128 : nr;
      assert(len == fs_read(fd, buf, len));
      for (; len > 0; ) {
        off += len;
        nr -= len;
        memcpy((void*)va, buf, len);
        va = va + len;
        len = nr > 128 ? 128 : nr;
        fs_lseek(fd, off, SEEK_SET);
        assert(len == fs_read(fd, buf, len));

      }
      assert(va == phdr.p_vaddr + phdr.p_filesz);
      if (phdr.p_filesz < phdr.p_memsz) {
        nr = phdr.p_memsz - phdr.p_filesz;
        len = nr > 128 ? 128 : nr;
        fs_lseek(fd, off, SEEK_SET);
        assert(len == fs_read(fd, buf, len));
        for (; len > 0; ) {
          off += len;
          nr -= len;

          memset((void*)va, 0, len);
          va = va + len;

          len = nr > 128 ? 128 : nr;
          fs_lseek(fd, off, SEEK_SET);
          assert(len == fs_read(fd, buf, len));

        }
        assert(va == phdr.p_vaddr + phdr.p_memsz);

      }

      uintptr_t end = va; 
      if (end > max_end) {
        max_end = end;
      }


    }
    offset += elf.e_phentsize;
    fs_lseek(fd, offset, SEEK_SET);

  }

  pcb->max_brk = max_end;
  fs_close(fd);
  return elf.e_entry;
}

void naive_uload(PCB *pcb, const char *filename) {
  uintptr_t entry = loader(pcb, filename);
  Log("Jump to entry = %p", entry);
  ((void(*)())entry) ();
}

