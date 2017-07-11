/*********************************************************************************************************************/
/*                                                  /===-_---~~~~~~~~~------____                                     */
/*                                                 |===-~___                _,-'                                     */
/*                  -==\\                         `//~\\   ~~~~`---.___.-~~                                          */
/*              ______-==|                         | |  \\           _-~`                                            */
/*        __--~~~  ,-/-==\\                        | |   `\        ,'                                                */
/*     _-~       /'    |  \\                      / /      \      /                                                  */
/*   .'        /       |   \\                   /' /        \   /'                                                   */
/*  /  ____  /         |    \`\.__/-~~ ~ \ _ _/'  /          \/'                                                     */
/* /-'~    ~~~~~---__  |     ~-/~         ( )   /'        _--~`                                                      */
/*                   \_|      /        _)   ;  ),   __--~~                                                           */
/*                     '~~--_/      _-~/-  / \   '-~ \                                                               */
/*                    {\__--_/}    / \\_>- )<__\      \                                                              */
/*                    /'   (_/  _-~  | |__>--<__|      |                                                             */
/*                   |0  0 _/) )-~     | |__>--<__|     |                                                            */
/*                   / /~ ,_/       / /__>---<__/      |                                                             */
/*                  o o _//        /-~_>---<__-~      /                                                              */
/*                  (^(~          /~_>---<__-      _-~                                                               */
/*                 ,/|           /__>--<__/     _-~                                                                  */
/*              ,//('(          |__>--<__|     /                  .----_                                             */
/*             ( ( '))          |__>--<__|    |                 /' _---_~\                                           */
/*          `-)) )) (           |__>--<__|    |               /'  /     ~\`\                                         */
/*         ,/,'//( (             \__>--<__\    \            /'  //        ||                                         */
/*       ,( ( ((, ))              ~-__>--<_~-_  ~--____---~' _/'/        /'                                          */
/*     `~/  )` ) ,/|                 ~-_~>--<_/-__       __-~ _/                                                     */
/*   ._-~//( )/ )) `                    ~~-'_/_/ /~~~~~~~__--~                                                       */
/*    ;'( ')/ ,)(                              ~~~~~~~~~~                                                            */
/*   ' ') '( (/                                                                                                      */
/*     '   '  `                                                                                                      */
/*********************************************************************************************************************/
#ifndef _MDL_H_
#define _MDL_H_

#include "types.h"

#define MDL_MAGIC "MDL"
#define MDL_INVALID_OFFSET 0xFFFFFFFF

typedef struct {
    u32 offset;
    u32 size;
} data_chunk;

/* Model File */
struct mdl_file {
    /* Header */
    struct mdl_header {
        /* File identifier {0x4D, 0x44, 0x4C, 0x00} */
        byte id[4];
        /* Version {0x00, 0x01} */
        struct {
            u16 maj;
            u16 min;
        } ver;
        /* Flags:
         *  Bit 0: Rigged
         *  Bit 1-31: Unused */
        struct {
            int rigged : 1;
            int unused : 31;
        } flags;
        /* Number of vertices (total) */
        u32 num_vertices;
        /* Number of indices (total) */
        u32 num_indices;
        /* Number of mesh descriptors */
        u16 num_mesh_descs;
        /* Number of joints */
        u16 num_joints;
        /* Number of strings */
        u32 num_strings;
        /* Data chunk of mesh descriptors */
        data_chunk mesh_descs;
        /* Data chunk of vertices */
        data_chunk verts;
        /* Data chunk of weights */
        data_chunk weights;
        /* Data chunk of indices */
        data_chunk indices;
        /* Data chunk of joints */
        data_chunk joints;
        /* Data chunk of array of joint name offsets */
        data_chunk joint_name_ofs;
        /* Data chunk of strings */
        data_chunk strings;
    } header;

    /* Array of mesh descriptors */
    struct mdl_mesh_desc {
        /* Name offset in strings */
        u32 ofs_name;
        /* Number of vertices */
        u32 num_vertices;
        /* Number of indices */
        u32 num_indices;
        /* Offset to array of vertices (relative to verts field) */
        u32 ofs_verts;
        /* Offset to array of weights (relative to weights field) */
        u32 ofs_weights;
        /* Offset to array of indices (relative to indices field) */
        u32 ofs_indices;
        /* Material reference index */
        u16 mat_idx;
    }* mesh_desc;

    /* Array of vertices */
    struct mdl_vertex {
        /* Position */
        f32 position[3];
        /* Normal */
        f32 normal[3];
        /* UV */
        f32 uv[2];
    }* vertices;

    /* Array of joint weights
     * (only if bit 0 is set) */
    struct mdl_vertex_weight {
        /* Blend indices */
        u16 blend_ids[4];
        /* Blend weights */
        f32 blend_weights[4];
    }* weights;

    /* Array of indices */
    u32* indices;

    /* Array of joints
     *  - Transforms are absolute, not relative to parent */
    struct mdl_joint {
        /* Reference to parent (index to current array) */
        u32 ref_parent;
        /* Position */
        f32 position[3];
        /* Rotation (quaternion) */
        f32 rotation[4];
        /* Scaling */
        f32 scaling[3];
    }* joints;

    /* Array of joint name offsets */
    u32* joint_name_ofs;

    /* Strings data
     *  - Contains contatenated list of strings null terminated strings */
    const char* strings;
};

void mdl_parse_from_buf(struct mdl_file* m, byte* buf, u32 buf_sz);

#endif /* ! _MDL_H_ */
