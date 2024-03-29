#include "render_world.h"
#include "flat_shader.h"
#include "object.h"
#include "light.h"
#include "ray.h"


Render_World::Render_World()
    :background_shader(0),ambient_intensity(0),enable_shadows(true),
    recursion_depth_limit(3)
{}

Render_World::~Render_World()
{
    delete background_shader;
    for(size_t i=0;i<objects.size();i++) delete objects[i];
    for(size_t i=0;i<lights.size();i++) delete lights[i];
}

// Find the closest object of intersection and return the object that was
// intersected.  Record the Hit structure in hit.  If no intersection occurred,
// return NULL.  Note that in the case of a Boolean, the object returned will be
// the Boolean, but the object stored in hit will be the underlying primitive.
// Any intersection with t<=small_t should be ignored.
Object* Render_World::Closest_Intersection(const Ray& ray, Hit& hit)
{
	double minT = (std::numeric_limits<double>::max());
	
	Object* ClosestObject = NULL;
	bool NoHit = true;
	
	for(int i = 0; i < objects.size(); i++)
	{
		std::vector<Hit> hits;
		objects.at(i)->Intersection(ray, hits);
		for(int j = 0; j < hits.size(); j++)
		{
			if((hits.at(j).t < minT) && (hits.at(j).t >= small_t))
			{
				minT = hits.at(j).t;
				hit = hits.at(j);
				ClosestObject = objects.at(i);
				NoHit = false;
			}
		}
	}
	if(NoHit == true)
	{
			return NULL;
	}

    // TODO
    return ClosestObject;
}

// set up the initial view ray and call
void Render_World::Render_Pixel(const ivec2& pixel_index)
{
    Ray ray; // TODO: set up the initial view ray here
    ray.direction = (camera.World_Position(pixel_index) - camera.position).normalized();
    ray.endpoint = camera.position;
    vec3 color=Cast_Ray(ray,1);
    camera.Set_Pixel(pixel_index,Pixel_Color(color));
}

void Render_World::Render()
{
    for(int j=0;j<camera.number_pixels[1];j++)
        for(int i=0;i<camera.number_pixels[0];i++)
            Render_Pixel(ivec2(i,j));
}

// cast ray and return the color of the closest intersected surface point,
// or the background color if there is no object intersection
vec3 Render_World::Cast_Ray(const Ray& ray,int recursion_depth)
{
    // TODO
    Hit hits;
    Object* IntObject;
    IntObject = Render_World::Closest_Intersection(ray, hits);
    vec3 color;
    if(IntObject != NULL)
    {
		vec3 dummy1;
		vec3 intersect = ray.Point(hits.t);
		vec3 normal = IntObject->Normal(intersect);
		color = IntObject->material_shader->Shade_Surface(ray, intersect, normal, 1);
	}
	else
	{
		vec3 dummy1;
		color = background_shader->Shade_Surface(ray, dummy1, dummy1, 1);
	}

    // determine the color here

    return color;
}
