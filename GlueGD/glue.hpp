/* GlueGD - A library that manages Geometry Dash mods
 * Copyright (C) 2021 Aly Cerruti
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <https://www.gnu.org/licenses/>. */
#pragma once

#include <filesystem>
#include <map>
#include <string>

#include <sol/sol.hpp>

// Module represents what's returned from the Lua, which is location-independent
struct Module {
    sol::protected_function on_enable;
};

template<typename Handler>
bool sol_lua_check(
    sol::types<Module>, lua_State* L,
    int index, Handler&& handler,
    sol::stack::record& tracking
);

Module sol_lua_get(
    sol::types<Module>, lua_State* L,
    int index, sol::stack::record& tracking
);

// Module entry represents what's stored, containing additional state data that
// assists with reloads, code patches, etc.
struct ModuleEntry : Module {
    std::filesystem::path loaded_from;
};

class Glue {
    sol::state lua;
    // Even though the order doesn't matter, we want a sorted map for UX.
    std::map<std::string, ModuleEntry> m_modules;

public:
    Glue();

    void load_module(std::string name, std::filesystem::path load_from);
    void enable_module(std::string name);

    const std::map<std::string, ModuleEntry>& modules() const;
};
