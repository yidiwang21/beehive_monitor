#ifndef _PDB_H_
#define _PDB_H_

#define PDB_CONFIG (PDB_SC_TRGSEL( 15 ) | PDB_SC_PDBEN | PDB_SC_CONT | PDB_SC_PDBIE | PDB_SC_DMAEN)

void PDB_setup() {
    // enable pdb clk
    SIM_SCGC6 |= SIM_SCGC6_PDB;
    PDB0_SC = PDB_SC_TRGSEL(0xf) | PDB_SC_PDBEN | PDB_SC_CONT | PDB_SC_PDBIE | PDB_SC_DMAEN;

    PDB0_SC |= PDB_SC_SWTRIG;
    PDB0_SC |= PDB_SC_LDOK;
}

void disablePDB() {
    PDB0_SC = 0;
}

void enablePDB() {
    PDB0_SC = PDB_CONFIG | PDB_SC_SWTRIG;
    PDB0_SC = PDB_CONFIG | PDB_SC_LDOK;
}


#endif