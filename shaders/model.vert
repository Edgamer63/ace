#version 330 core
layout (location = 0) in vec3 pos;
layout (location = 1) in vec3 vertex_color;
layout (location = 2) in vec3 normal;
layout (location = 3) in vec3 kv6norm;

out vec3 color;
out float diffuse;
out float fog;

layout (std140) uniform SceneUniforms {
    mat4 view;
    mat4 proj;
    mat4 pv;
    vec3 cam_forward;
    vec3 cam_right;
    vec3 cam_up;
    vec3 fog_color;
    vec3 light_pos;
};

uniform mat4 model;
uniform mat3 normal_matrix;
uniform bool local;

uniform vec3 filter_color = vec3(0.0);
uniform vec3 replacement_color;

void main() {
    vec4 model_space = model * vec4(pos, 1.0);
    gl_Position = (local ? proj : pv) * model_space;

    float kv6diffuse = dot(normalize(normal_matrix * kv6norm), light_pos);
    diffuse = max(dot(normalize(normal_matrix * normal), light_pos) + kv6diffuse, 0.0);

    color = vertex_color == filter_color ? replacement_color : vertex_color;
    fog = int(!local) * (1.0 - clamp((128 - length((view * model_space).xyz)) / 64, 0.0, 1.0));
}