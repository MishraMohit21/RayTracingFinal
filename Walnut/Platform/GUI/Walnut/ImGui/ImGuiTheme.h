#pragma once

#include <algorithm>
#include <imgui.h>
#include <glm/glm.hpp>

namespace Walnut::UI {

	namespace Colors
	{
		static inline float Convert_sRGB_FromLinear(float theLinearValue);
		static inline float Convert_sRGB_ToLinear(float thesRGBValue);
		ImVec4 ConvertFromSRGB(ImVec4 colour);
		ImVec4 ConvertToSRGB(ImVec4 colour);

		// To experiment with editor theme live you can change these constexpr into static
		// members of a static "Theme" class and add a quick ImGui window to adjust the colour values
		namespace Theme
		{
			constexpr auto accent = IM_COL32(255, 142, 69, 255);          // A warm orange accent
			constexpr auto highlight = IM_COL32(87, 187, 255, 255);       // A bright cyan for highlights
			constexpr auto niceBlue = IM_COL32(126, 214, 255, 255);       // A light sky blue
			constexpr auto compliment = IM_COL32(89, 165, 173, 255);      // A soft teal
			constexpr auto background = IM_COL32(30, 30, 30, 255);        // Dark background, softened slightly
			constexpr auto backgroundDark = IM_COL32(18, 18, 18, 255);    // Very dark, to contrast other colors
			constexpr auto titlebar = IM_COL32(28, 28, 28, 255);          // Darker titlebar, not pure black
			constexpr auto propertyField = IM_COL32(20, 20, 20, 255);     // Dark gray for fields
			constexpr auto text = IM_COL32(200, 200, 200, 255);           // Slightly brighter than before for readability
			constexpr auto textBrighter = IM_COL32(230, 230, 230, 255);   // Bright text for important areas
			constexpr auto textDarker = IM_COL32(150, 150, 150, 255);     // More muted text color
			constexpr auto textError = IM_COL32(255, 70, 70, 255);        // A brighter red for errors
			constexpr auto muted = IM_COL32(90, 90, 90, 255);             // Slightly brighter muted areas
			constexpr auto groupHeader = IM_COL32(60, 60, 60, 255);       // Darker header for sections
			constexpr auto selection = IM_COL32(245, 198, 134, 255);      // Warm, pleasant orange for selections
			constexpr auto selectionMuted = IM_COL32(245, 218, 156, 255); // Muted version of the selection color
			constexpr auto backgroundPopup = IM_COL32(60, 60, 60, 255);   // A slightly brighter popup background
			constexpr auto validPrefab = IM_COL32(100, 200, 220, 255);    // Cool cyan for valid prefabs
			constexpr auto invalidPrefab = IM_COL32(245, 100, 100, 255);  // Bright red for invalid prefabs
			constexpr auto missingMesh = IM_COL32(255, 120, 100, 255);    // Warm coral color for missing meshes
			constexpr auto meshNotSet = IM_COL32(255, 160, 90, 255);      // Soft orange for unset meshes

		}
	}

	namespace Colors
	{
		inline float Convert_sRGB_FromLinear(float theLinearValue)
		{
			return theLinearValue <= 0.0031308f
				? theLinearValue * 12.92f
				: glm::pow<float>(theLinearValue, 1.0f / 2.2f) * 1.055f - 0.055f;
		}

		inline float Convert_sRGB_ToLinear(float thesRGBValue)
		{
			return thesRGBValue <= 0.04045f
				? thesRGBValue / 12.92f
				: glm::pow<float>((thesRGBValue + 0.055f) / 1.055f, 2.2f);
		}

		inline ImVec4 ConvertFromSRGB(ImVec4 colour)
		{
			return ImVec4(Convert_sRGB_FromLinear(colour.x),
				Convert_sRGB_FromLinear(colour.y),
				Convert_sRGB_FromLinear(colour.z),
				colour.w);
		}

		inline ImVec4 ConvertToSRGB(ImVec4 colour)
		{
			return ImVec4(std::pow(colour.x, 2.2f),
				glm::pow<float>(colour.y, 2.2f),
				glm::pow<float>(colour.z, 2.2f),
				colour.w);
		}

		inline ImU32 ColorWithValue(const ImColor& color, float value)
		{
			const ImVec4& colRow = color.Value;
			float hue, sat, val;
			ImGui::ColorConvertRGBtoHSV(colRow.x, colRow.y, colRow.z, hue, sat, val);
			return ImColor::HSV(hue, sat, std::min(value, 1.0f));
		}

		inline ImU32 ColorWithSaturation(const ImColor& color, float saturation)
		{
			const ImVec4& colRow = color.Value;
			float hue, sat, val;
			ImGui::ColorConvertRGBtoHSV(colRow.x, colRow.y, colRow.z, hue, sat, val);
			return ImColor::HSV(hue, std::min(saturation, 1.0f), val);
		}

		inline ImU32 ColorWithHue(const ImColor& color, float hue)
		{
			const ImVec4& colRow = color.Value;
			float h, s, v;
			ImGui::ColorConvertRGBtoHSV(colRow.x, colRow.y, colRow.z, h, s, v);
			return ImColor::HSV(std::min(hue, 1.0f), s, v);
		}

		inline ImU32 ColorWithMultipliedValue(const ImColor& color, float multiplier)
		{
			const ImVec4& colRow = color.Value;
			float hue, sat, val;
			ImGui::ColorConvertRGBtoHSV(colRow.x, colRow.y, colRow.z, hue, sat, val);
			return ImColor::HSV(hue, sat, std::min(val * multiplier, 1.0f));
		}

		inline ImU32 ColorWithMultipliedSaturation(const ImColor& color, float multiplier)
		{
			const ImVec4& colRow = color.Value;
			float hue, sat, val;
			ImGui::ColorConvertRGBtoHSV(colRow.x, colRow.y, colRow.z, hue, sat, val);
			return ImColor::HSV(hue, std::min(sat * multiplier, 1.0f), val);
		}

		inline ImU32 ColorWithMultipliedHue(const ImColor& color, float multiplier)
		{
			const ImVec4& colRow = color.Value;
			float hue, sat, val;
			ImGui::ColorConvertRGBtoHSV(colRow.x, colRow.y, colRow.z, hue, sat, val);
			return ImColor::HSV(std::min(hue * multiplier, 1.0f), sat, val);
		}
	}

	void SetHazelTheme();

}