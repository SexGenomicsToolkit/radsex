#pragma once
#include <stdio.h>
#include <zlib.h>
#include <string.h>
#include <errno.h>
#include <assert.h>
#include <string>
#include "utils.h"
#include "parameters.h"
#include "popmap_file.h"
#include "output.h"
#include "bwa/bwamem.h"
#include "bwa/kseq.h"


void mapping(Parameters& parameters);
