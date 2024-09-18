#include "Walnut/Application.h"
#include "Walnut/EntryPoint.h"

#include "Walnut/Image.h"
#include "Walnut/Timer.h"

#include "Renderer.h"
#include "Camera.h"

#include <glm/gtc/type_ptr.hpp>

using namespace Walnut;

class ExampleLayer : public Walnut::Layer
{
public:
	ExampleLayer()
		: m_Camera(45.0f, 0.1f, 100.0f)
	{
		// Materials
		{
			// Blue Material
			Material material;
			material.Albedo = { 0.2f, 0.3f, 1.0f };
			material.Roughness = 0.1;
			material.Metallic = 0.0;
			m_Scene.Materials.push_back(material);
		}

		{
			//Pink Material
			Material material;
			material.Albedo = { 1.0f, 0.0f, 1.0f };
			material.Roughness = 0.0;
			material.Metallic = 1.0;
			m_Scene.Materials.push_back(material);
		}

		{
			// Orange Material
			Material material;
			material.Albedo = { 0.8f, 0.5f, 0.2f };
			material.Roughness = 0.1;
			material.Metallic = 0.0;
			material.EmissionColor = material.Albedo;
			material.EmissionStrength = 2.0f;
			m_Scene.Materials.push_back(material);
		}


		// Spehre
		{
			Sphere sphere; // Ground
			sphere.Position = { 0.0f, -101.0f, 0.0f };
			sphere.Radius = 100.0f;
			sphere.MaterialIndex = 1;
			m_Scene.Spheres.push_back(sphere);
		}

		{
			Sphere sphere;
			sphere.Position = { 0.0f, 0.0f, 0.0f };
			sphere.Radius = 1.0f;
			sphere.MaterialIndex = 2;
			m_Scene.Spheres.push_back(sphere);
		}


		{
			Sphere sphere; // Sun
			sphere.Position = { 4.0f, 0.0f, 0.0f };
			sphere.Radius = 1.0f;
			sphere.MaterialIndex = 3;
			m_Scene.Spheres.push_back(sphere);
		}

	}

	virtual void OnUpdate(float ts) override
	{
		if (m_Camera.OnUpdate(ts))
			m_Renderer.ResetFrameIndex();
	}

	virtual void OnUIRender() override
	{
		ImGui::Begin("Settings");
		ImGui::Text("Last render: %.3fms", m_LastRenderTime);
		if (ImGui::Button("Render"))
		{
			Render();
		}
		ImGui::Separator();
		if (ImGui::Button("Reset"))
			m_Renderer.ResetFrameIndex();
		ImGui::Separator();
		ImGui::Checkbox("Accumalate", &m_Renderer.GetSetting().Accumulate);
		ImGui::Separator();
		ImGui::Checkbox("Multi Threading", &m_Renderer.GetSetting().multiThreading);

		ImGui::End();

		ImGui::Begin("Scene");
			ImGui::Text("Spheres");
		for (size_t i = 0; i < m_Scene.Spheres.size(); i++)
		{
			ImGui::PushID(i);
			Sphere& sphere = m_Scene.Spheres[i];
			ImGui::DragFloat3("Position", glm::value_ptr(sphere.Position), 0.1f);
			ImGui::DragFloat("Radius", &sphere.Radius, 0.1f);
			ImGui::DragInt("Material", &sphere.MaterialIndex, 1.0f, 0, m_Scene.Materials.size() - 1);
			ImGui::Separator();

			ImGui::PopID();
		}

			ImGui::Text("Materials");
		for (size_t i = 1; i < m_Scene.Materials.size(); i++)
		{
			ImGui::PushID(i);
			Material& material = m_Scene.Materials[i];
			ImGui::ColorEdit3("Albedo", glm::value_ptr(material.Albedo));
			ImGui::DragFloat("Rougness", &(material.Roughness));
			ImGui::DragFloat("Metallic", &(material.Metallic));
			ImGui::ColorEdit3("Emission Color", glm::value_ptr(material.EmissionColor));
			ImGui::DragFloat("Emission Strength", &(material.EmissionStrength), 0.1f, 0.0, FLT_MAX);

			
			ImGui::PopID();
		}

		//ImGui::Text("Number of Bounces");
		ImGui::Separator();
		ImGui::InputInt("Bounces", &m_Renderer.Pub_bounces);


		ImGui::End();

		ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0.0f, 0.0f));
		ImGui::Begin("Viewport");

		m_ViewportWidth = ImGui::GetContentRegionAvail().x;
		m_ViewportHeight = ImGui::GetContentRegionAvail().y;

		auto image = m_Renderer.GetFinalImage();
		if (image)
			ImGui::Image(image->GetDescriptorSet(), { (float)image->GetWidth(), (float)image->GetHeight() },
				ImVec2(0, 1), ImVec2(1, 0));

		ImGui::End();
		ImGui::PopStyleVar();

		Render();
	}

	void Render()
	{
		Timer timer;

		m_Renderer.OnResize(m_ViewportWidth, m_ViewportHeight);
		m_Camera.OnResize(m_ViewportWidth, m_ViewportHeight);
		m_Renderer.Render(m_Scene, m_Camera);

		m_LastRenderTime = timer.ElapsedMillis();
	}
private:
	Renderer m_Renderer;
	Camera m_Camera;
	Scene m_Scene;
	uint32_t m_ViewportWidth = 0, m_ViewportHeight = 0;

	float m_LastRenderTime = 0.0f;
};

Walnut::Application* Walnut::CreateApplication(int argc, char** argv)
{
	Walnut::ApplicationSpecification spec;
	spec.Name = "Ray Tracing";
	spec.CustomTitlebar = true; 
	Walnut::Application* app = new Walnut::Application(spec);
	app->PushLayer<ExampleLayer>();
	app->SetMenubarCallback([app]()
		{
			if (ImGui::BeginMenu("File"))
			{
				if (ImGui::MenuItem("Exit"))
				{
					app->Close();
				}
				ImGui::EndMenu();
			}
		});
	return app;
}