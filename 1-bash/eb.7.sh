#!/bin/bash
wget ftp://ftp.sra.ebi.ac.uk/vol1/fastq/SRR836/007/SRR8363277/SRR8363277_1.fastq.gz -O SRR8363277_1.fastq.gz
zcat SRR8363277_1.fastq.gz | head -c 128 | od -i | fold -w 4 | sort -n
