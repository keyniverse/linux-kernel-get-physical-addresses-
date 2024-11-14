#include <linux/kernel.h>
#include <linux/syscalls.h>
#include <linux/sched.h>
#include <linux/mm.h>  
#include <asm/pgtable.h>

SYSCALL_DEFINE1(my_get_physical_addresses, void *, addr_p) {


    unsigned long vaddr = (unsigned long)addr_p;
    pgd_t *pgd;
    p4d_t *p4d;
    pud_t *pud;
    pmd_t *pmd;
    pte_t *pte;
    
    unsigned long paddr = 0;
    unsigned long page_addr = 0;
    unsigned long page_offset = 0;
    
    // get PGD（Page Global Directory）
    pgd = pgd_offset(current->mm, vaddr);
    if (pgd_none(*pgd)) {
        printk("Invalid PGD.\n");
        return 0;
    }

    // get P4D
    p4d = p4d_offset(pgd, vaddr);
    if (p4d_none(*p4d)) {
        printk("Invalid P4D.\n");
        return 0;
    }

    // get PUD（Page Upper Directory）
    pud = pud_offset(p4d, vaddr);
    if (pud_none(*pud)) {
        printk("Invalid PUD.\n");
        return 0;
    }

    // get PMD（Page Middle Directory）
    pmd = pmd_offset(pud, vaddr);
    if (pmd_none(*pmd)) {
        printk("Invalid PMD.\n");
        return 0;
    }

    // get PTE（Page Table Entry）
    pte = pte_offset_kernel(pmd, vaddr);
    if (pte_none(*pte)) {
        printk("Invalid PTE.\n");
        return 0;
    }
    
    page_addr = pte_val(*pte) & PAGE_MASK & PTE_PFN_MASK;
    page_offset = vaddr & ~PAGE_MASK;
    paddr = page_addr | page_offset;

    printk("pgd Address:       0x%lx\n", pgd);
    printk("p4d Address:       0x%lx\n", p4d);
    printk("pud Address:       0x%lx\n", pud);
    printk("pmd Address:       0x%lx\n", pmd);
    printk("pte Address:       0x%lx\n", pte);
    printk("pte_value:         0x%lx\n", pte_val(*pte));
    printk("page_addr Address: 0x%lx\n", pte_val(*pte) & PAGE_MASK);

    

    printk("Page Address: 0x%lx, Page Offset: 0x%lx\n", page_addr, page_offset);
    printk("Virtual Address: 0x%lx, Physical Address: 0x%lx\n", vaddr, paddr);

    return paddr;
}