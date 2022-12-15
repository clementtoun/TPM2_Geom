#version 330 core
layout (triangles) in;
layout (triangle_strip, max_vertices = 3) out;

flat in uint out_isSelected[];
in vec3 out_FragPos[];
in vec3 out_Normal[];
in float out_f[];

flat out uint isSelected;
out vec3 FragPos;
out vec3 Normal;
out vec3 dist;
out float f;

uniform vec2 WIN_SCALE;

void main() {

    uint selected = (uint(1) - out_isSelected[0]) * (uint(1) - out_isSelected[1]) * (uint(1) - out_isSelected[2]);

    vec2 p0 = WIN_SCALE * gl_in[0].gl_Position.xy / gl_in[0].gl_Position.w;
    vec2 p1 = WIN_SCALE * gl_in[1].gl_Position.xy / gl_in[1].gl_Position.w;
    vec2 p2 = WIN_SCALE * gl_in[2].gl_Position.xy / gl_in[2].gl_Position.w;

    vec2 v0 = p2-p1;
    vec2 v1 = p2-p0;
    vec2 v2 = p1-p0;
    float area = abs(v1.x*v2.y - v1.y * v2.x);

    isSelected = selected;
    gl_Position = gl_in[0].gl_Position;
    dist = vec3(area/length(v0),0,0);
    FragPos = out_FragPos[0];
    Normal = out_Normal[0];
    f = out_f[0];
    EmitVertex();

    isSelected = selected;
    gl_Position = gl_in[1].gl_Position;
    dist = vec3(0,area/length(v1),0);
    FragPos = out_FragPos[1];
    Normal = out_Normal[1];
    f = out_f[1];
    EmitVertex();

    isSelected = selected;
    gl_Position = gl_in[2].gl_Position;
    dist = vec3(0,0,area/length(v2));
    FragPos = out_FragPos[2];
    Normal = out_Normal[2];
    f = out_f[2];
    EmitVertex();

    EndPrimitive();
}