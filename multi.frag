#version 410
#extension GL_ARB_compute_shader : enable
#extension GL_ARB_shader_storage_buffer_object : enable

in vec4 fPos;
out vec4 fragColor;

uniform float width;
uniform float height;

uniform int numParticles;
// multiple particles
struct particle {
    vec3 position;
};

layout(std430, binding = 3) buffer particleBuffer
{
    particle p[];
};


vec2 iResolution =vec2(width,height);
vec2 fragCoord = vec2((fPos.x + 1.0f)/2.0f*iResolution.x, (fPos.y + 1.0f)/2.0f*iResolution.y);


float map(vec3 pos){
    float minimum = 1000.0f;
    float d;
    for(int i=0; i<numParticles; i++){

        d = length(pos - p[i].position)- 0.5f;
        minimum = min(minimum,d);
    }
    return minimum;
}

vec3 calcNormal(vec3 pos){
    vec2 e = vec2(0.0001f,0.0f);
    
    return normalize(vec3(map(pos+e.xyy) - map(pos-e.xyy),
                            map(pos+e.yxy) - map(pos-e.yxy),
                            map(pos+e.yyx) - map(pos-e.yyx)));
}

float castRay(vec3 ro, vec3 rd){
    float t = 0.0f;
    vec3 pos;
    for(int i=0; i<100;i++){
        pos = ro + t*rd;
        float h = map(pos);

        if(h<0.001f){
            break;
        }

        t+= h;

        if(t>40.0f){
            break;
        }

        
    }

    if(t>40.0){
        t=-1.0f;
    }


    return t;
}



void main(){
    
    vec3 col = vec3(0.0f);

    vec2 p = (2.0*fragCoord - iResolution.xy)/iResolution.y;
    //p = fPos.xy;
    vec3 ro = vec3(0.0f,1.5f,15.0f);
    vec3 rd = normalize(vec3(p,-1.5f));




    
    float t = castRay(ro, rd);

    if(t>0.0f){
        col = vec3(1.0f);
        vec3 pos = ro + t*rd;
        vec3 nor = calcNormal(pos);
        vec3 key_dir = normalize(vec3(0.7f, 0.3f,0.2f));

        float key_dif = clamp(dot(nor,key_dir),0.0f, 1.0f);
        float key_shadow = step(castRay(pos + 0.001f*nor, key_dir),0.0f);
        float sky_dif = clamp(0.5 +dot(nor, vec3(0.0f,1.0f,0.1f)),0.0f, 1.0f);

        //col = vec3(abs(nor.x), abs(nor.y),abs(nor.z));
        col = vec3(1.5f, 0.9f,0.7f)*key_dif*key_shadow;
        col += vec3(0.0f, 0.3f, 0.4f)*sky_dif;
    }
         

    col = pow(col,vec3(0.4545));
    fragColor = vec4(col,1.0f);
}