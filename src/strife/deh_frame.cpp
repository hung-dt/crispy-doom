//
// Copyright(C) 2005-2014 Simon Howard
//
// This program is free software; you can redistribute it and/or
// modify it under the terms of the GNU General Public License
// as published by the Free Software Foundation; either version 2
// of the License, or (at your option) any later version.
//
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.
//
//
// Parses "Frame" sections in dehacked files
//

#include <cstdio>
#include <cstdlib>

#include "doomtype.h"
#include "d_items.h"
#include "info.h"

#include "deh_defs.h"
#include "deh_io.h"
#include "deh_main.h"
#include "deh_mapping.h"

DEH_BEGIN_MAPPING(state_mapping, state_t)
  DEH_MAPPING("Sprite number",    sprite)
  DEH_MAPPING("Sprite subnumber", frame)
  DEH_MAPPING("Duration",         tics)
  DEH_MAPPING("Next frame",       nextstate)
  DEH_UNSUPPORTED_MAPPING("Action pointer")
DEH_END_MAPPING

static void *DEH_FrameStart(deh_context_t *context, char *line)
{
    int frame_number = 0;
    state_t *state;
    
    if (sscanf(line, "Frame %i", &frame_number) != 1)
    {
        DEH_Warning(context, "Parse error on section start");
        return nullptr;
    }
    
    if (frame_number < 0 || frame_number >= NUMSTATES)
    {
        DEH_Warning(context, "Invalid frame number: %i", frame_number);
        return nullptr;
    }

    if (frame_number >= DEH_VANILLA_NUMSTATES) 
    {
        DEH_Warning(context, "Attempt to modify frame %i: this will cause "
                             "problems in Vanilla dehacked.", frame_number);
    }

    state = &states[frame_number];

    return state;
}

static void DEH_FrameParseLine(deh_context_t *context, char *line, void *tag)
{
    state_t *state;
    char *variable_name, *value;
    int ivalue;
    
    if (tag == nullptr)
       return;

    state = (state_t *) tag;

    // Parse the assignment

    if (!DEH_ParseAssignment(line, &variable_name, &value))
    {
        // Failed to parse

        DEH_Warning(context, "Failed to parse assignment");
        return;
    }
    
    // all values are integers

    ivalue = atoi(value);
    
    // set the appropriate field

    DEH_SetMapping(context, &state_mapping, state, variable_name, ivalue);
}

static void DEH_FrameSHA1Sum(sha1_context_t *context)
{
    int i;

    for (i=0; i<NUMSTATES; ++i)
    {
        DEH_StructSHA1Sum(context, &state_mapping, &states[i]);
    }
}

deh_section_t deh_section_frame =
{
    "Frame",
    nullptr,
    DEH_FrameStart,
    DEH_FrameParseLine,
    nullptr,
    DEH_FrameSHA1Sum,
};

