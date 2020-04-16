uniform sampler2D texture;
uniform vec2 light_pos;
uniform float samples;
uniform float light_range;
uniform vec2 dimensions;
uniform vec2 delta;

void main()
{
	float aspect = dimensions.x/dimensions.y;
	vec2 light_pos_d = light_pos-delta;
	vec2 pos = vec2(gl_TexCoord[0].x, gl_TexCoord[0].y) - delta;
	float dist = distance(light_pos_d, pos);
	float intensity = mix(1.0, 0.0, dist/(vec2(light_pos_d.x*aspect,light_pos_d)*light_range));
	int samples_n = floor(dist/samples);
	if(intensity > 0.2)
	{
		for(float i = dist; i > 0; i-=samples)
		{
			intensity = min(intensity, texture2D(texture, mix(light_pos, gl_TexCoord[0].xy, i / dist)));
		}
	}
	gl_FragColor = intensity * texture2D(texture, gl_TexCoord[0].xy);
}