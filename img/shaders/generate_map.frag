uniform sampler2D texture;
uniform vec2 light_pos;
uniform float samples;

void main()
{
	float intensity = 1;
	for(int i = 0; i < samples; i++)
	{
		intensity = min(intensity, texture2D(texture, mix(light_pos, gl_TexCoord[0].xy, float(i + 0.5) / samples)));
	}
	gl_FragColor = intensity * texture2D(texture, gl_TexCoord[0].xy);
}