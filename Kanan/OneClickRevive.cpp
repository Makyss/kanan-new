#include <imgui.h>

#include <Scan.hpp>

#include "Log.hpp"
#include "OneClickRevive.hpp"

using namespace std;

namespace kanan {
    OneClickRevive::OneClickRevive()
        : m_isEnabled{ false },
        m_patch{}
    {
        auto address = scan("client.exe", "0F 86 ? ? ? ? 8B 4D E8 8B 11");

        if (address) {
            log("Got OneClickRevive %p", *address);

            m_patch.address = *address;
            m_patch.bytes = { 0x90, 0xE9 };
        }
        else {
            log("Failed to find OneClickRevive address.");
        }
    }

    void OneClickRevive::onPatchUI() {
        if (m_patch.address == 0) {
            return;
        }

        if (ImGui::Checkbox("One Click Revive", &m_isEnabled)) {
            apply();
        }

        if (ImGui::IsItemHovered()) {
            ImGui::SetTooltip("Credits: Step29");
        }
    }

    void OneClickRevive::onConfigLoad(const Config& cfg) {
        m_isEnabled = cfg.get<bool>("OneClickRevive.Enabled").value_or(false);

        if (m_isEnabled) {
            apply();
        }
    }

    void OneClickRevive::onConfigSave(Config& cfg) {
        cfg.set<bool>("OneClickRevive.Enabled", m_isEnabled);
    }

    void OneClickRevive::apply() {
        if (m_patch.address == 0) {
            return;
        }

        log("Toggling OneClickRevive...");

        if (m_isEnabled) {
            patch(m_patch);
        }
        else {
            undoPatch(m_patch);
        }
    }
}
