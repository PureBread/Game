#version 150

in vec2 Texcoord;

out vec4 outColor;

uniform sampler2D texFramebuffer;

float rand(vec2 co){
    return fract(sin(dot(co.xy ,vec2(12.9898,78.233))) * 43758.5453);
}

uniform float time = 0;

vec4 stuff4(vec2 uv){
	float xd = (0.75 - abs(uv.x-0.5)-0.25);
	float yd = (0.75 - abs(uv.y-0.5)-0.25);

	uv.x += sin(time*0.1001f)*0.1f*xd;
	uv.y += sin(time*0.0501f)*0.1f*yd;


	vec4 res = texture(texFramebuffer, uv)*4;

	float d = (0.5 - uv.y)*0.1;

	uv.y = (uv.y - (0.1f*sin(time))*d);
	res += texture(texFramebuffer, uv)*3;

	uv.y = (uv.y - (0.1f*sin(time))*d);
	res += texture(texFramebuffer, uv)*2;

	uv.y = (uv.y - (0.1f*sin(time))*d);
	res += texture(texFramebuffer, uv);


	return res/10;
}

void main() {
	vec2 v = vec2(Texcoord.x, rand(Texcoord));
	float d = distance(Texcoord, vec2(0.5));
    outColor = vec4(stuff4(Texcoord));
}
