#version 330

in vec4 color;
in vec3 fNormal;
in vec3 fPosition;

uniform mat4 model;

uniform float time;
uniform float rand;

const float ambientCoeff   = 0.1;

uniform vec3 LightColor;
uniform vec3 LightPosition;
uniform vec3 ViewPosition;

uniform float smoothness = 1.0;

in vec2 TexCoord;
uniform sampler2D texture_diffuse1;
uniform sampler2D texture_normal1;



// https://www.shadertoy.com/view/ldB3zc
float hash1( float n ) { return fract(sin(n)*43758.5453); }
vec2  hash2( vec2  p ) { p = vec2( dot(p,vec2(127.1,311.7)), dot(p,vec2(269.5,183.3)) ); return fract(sin(p)*43758.5453); }

vec4 voronoi(vec2 x, float w )
{
    vec2 n = floor( x );
    vec2 f = fract( x );

	vec4 m = vec4( 8.0, 0.0, 0.0, 0.0 );
    for( int j=-2; j<=2; j++ )
    for( int i=-2; i<=2; i++ )
    {
        vec2 g = vec2( float(i),float(j) );
        vec2 o = hash2( n + g );
		
		// animate
        o = 0.5 + 0.5*sin( time * 10 + 6.2831*o );

        // distance to cell		
		float d = length(g - f + o);
		
        // cell color
		vec3 col = 0.5 + 0.5*sin( hash1(dot(n+g,vec2(7.0,113.0)))*2.5 + 3.5 + vec3(2.0,3.0,0.0));
        // in linear space
        col = col*col;
        
        // do the smooth min for colors and distances		
		float h = smoothstep( -1.0, 1.0, (m.x-d)/w );
	    m.x   = mix( m.x,     d, h ) - h*(1.0-h)*w/(1.0+3.0*w); // distance
		m.yzw = mix( m.yzw, col, h ) - h*(1.0-h)*w/(1.0+3.0*w); // color
    }
	
	return m;
}


void main()                                                             
{

    // Ambient
    vec3 ambient = ambientCoeff * LightColor;


    // Diffuse
    float roughness = 0.5;
    vec3 lightDir = normalize(LightPosition - fPosition);
    float diff = max(dot(fNormal, lightDir), 0.0);
    vec3 diffuse = roughness * diff * LightColor;


    //Specular
    float specularStrength = 0.4;
    float shininess = 1.0;
    vec3 viewDir = normalize(ViewPosition-fPosition);
    vec3 reflectDir = reflect(-lightDir, fNormal);
    float spec = pow(max(dot(viewDir, reflectDir), 0.0),32);
    vec3 specular = shininess * specularStrength * spec * LightColor;
    
    vec3 ObjectColor;
 
    float x = (fPosition.x);
    float y = (fPosition.y);
    vec2 input = vec2(x,y);

    vec4 v = voronoi(input, smoothness);

    // gamma
    ObjectColor = sqrt(v.yzw);

    vec3 result = (ambient + diffuse + specular) * ObjectColor;
    gl_FragColor = vec4(result, 1.0);
}