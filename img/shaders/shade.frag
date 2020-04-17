uniform vec2 light_pos;
uniform sampler2D texture;
uniform float aspect;
uniform float dark;
uniform vec2 delta;

void main()
{
	vec4 color = texture2D(texture, gl_TexCoord[0].xy);
	vec2 light_pos_d = vec2(light_pos.x*aspect, light_pos.y);
	vec2 pos = vec2(gl_TexCoord[0].x*aspect, gl_TexCoord[0].y);
	if(color.w == 1.0)
	{
		float intensity = max(0.2, 1-distance(pos, light_pos_d));
		gl_FragColor = vec4(color.xyz,intensity);
	}
	else
	{
		gl_FragColor = vec4(vec3(dark), 1.0);
	}
}