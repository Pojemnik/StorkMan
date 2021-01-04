//out vec4 FragColor;
  
//in vec2 TexCoords;

uniform sampler2D gPosition;
uniform sampler2D gNormal;
uniform sampler2D gAlbedo;

const int NR_LIGHTS = 1;
uniform vec3 lights_color[NR_LIGHTS];
uniform vec3 lights_position[NR_LIGHTS];
uniform vec3 viewPos;

void main()
{ 
	vec2 TexCoords=gl_TexCoord[0].xy;
	
    // retrieve data from G-buffer
    vec3 FragPos = texture(gPosition, TexCoords).rgb;
    vec3 Normal = normalize(texture(gNormal, TexCoords).rgb-vec3(0.5,0.5,0.5));
    vec3 Albedo = texture(gAlbedo,TexCoords).rgb;
	
    // ambient
    vec3 lighting = Albedo * 0.1; // hard-coded ambient component
    vec3 viewDir = normalize(viewPos - FragPos);
	
	
	vec3 specularStrength = vec3(0.15, 0.04, 0.04);
    for(int i = 0; i < NR_LIGHTS; ++i)
    {
        // diffuse
        vec3 lightDir = normalize(lights_position[i] - FragPos);
        vec3 diffuse = max(dot(Normal, lightDir), 0.0) * Albedo * lights_color[i];
		
		// specular
		vec3 reflectDir = reflect(-lightDir, Normal);  
		float spec = pow(max(dot(viewDir, reflectDir), 0.0), 10);
		vec3 specular = specularStrength * spec * lights_color[i];
		
        lighting += diffuse + specular;
    }
    
    gl_FragColor = vec4(lighting, 1.0);
}  