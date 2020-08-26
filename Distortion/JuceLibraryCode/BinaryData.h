/* =========================================================================================

   This is an auto-generated file: Any edits you make may be overwritten!

*/

#pragma once

namespace BinaryData
{
    extern const char*   ABS_png;
    const int            ABS_pngSize = 315527;

    extern const char*   ABS1_png;
    const int            ABS1_pngSize = 1402523;

    extern const char*   ABS2_png;
    const int            ABS2_pngSize = 1280628;

    extern const char*   background_png;
    const int            background_pngSize = 225757;

    extern const char*   maxresdefaultBen_png;
    const int            maxresdefaultBen_pngSize = 161180;

    extern const char*   VU_meter_png;
    const int            VU_meter_pngSize = 65003;

    extern const char*   vuback_png;
    const int            vuback_pngSize = 49080;

    // Number of elements in the namedResourceList and originalFileNames arrays.
    const int namedResourceListSize = 7;

    // Points to the start of a list of resource names.
    extern const char* namedResourceList[];

    // Points to the start of a list of resource filenames.
    extern const char* originalFilenames[];

    // If you provide the name of one of the binary resource variables above, this function will
    // return the corresponding data and its size (or a null pointer if the name isn't found).
    const char* getNamedResource (const char* resourceNameUTF8, int& dataSizeInBytes);

    // If you provide the name of one of the binary resource variables above, this function will
    // return the corresponding original, non-mangled filename (or a null pointer if the name isn't found).
    const char* getNamedResourceOriginalFilename (const char* resourceNameUTF8);
}
