// Copyright 2021 Intel Corporation
// SPDX-License-Identifier: Apache-2.0

#include "SearchWidget.h"

#include "sg/visitors/Search.h"

#include <imgui.h>

SearchWidget::SearchWidget(
    std::vector<NT> &_searchTypes, std::vector<NT> &_displayTypes, TS ts)
    : searchTypes(_searchTypes), displayTypes(_displayTypes), displayState(ts)
{}

void SearchWidget::clear()
{
  searched = false;
  results.clear();
  searchTerm[0] = '\0';
  numPages = 0;
}

void SearchWidget::search(NR root)
{
  std::string searchStr(searchTerm);

  if (searchStr.size() <= 0) {
    clear();
    return;
  }

  searched = true;
  results.clear();
  for (auto nt : searchTypes)
    root.traverse<ospray::sg::Search>(searchStr, nt, results);

  numPages = results.size() / numItemsPerPage;
  numPages += results.size() % numItemsPerPage == 0 ? 0 : 1;
  paginateLabel = "of " + std::to_string(numPages) + "##currentPage";
  currentPage = 1;
}

void SearchWidget::addSearchBarUI(NR root)
{
  if (searched && lastRoot != &root)
    clear();
  lastRoot = &root;

  if (ImGui::InputTextWithHint("##findTransformEditor",
          "search...",
          searchTerm,
          1024,
          ImGuiInputTextFlags_CharsNoBlank | ImGuiInputTextFlags_AutoSelectAll
              | ImGuiInputTextFlags_EnterReturnsTrue)) {
    search(root);
  }
  ImGui::SameLine();
  if (ImGui::Button("find")) {
    search(root);
  }
  ImGui::SameLine();
  if (ImGui::Button("clear")) {
    clear();
  }
}

void SearchWidget::addSearchResultsUI(NR root)
{
  if (searched && lastRoot != &root)
    clear();
  lastRoot = &root;

  if (searched) {
    ImGui::SameLine();
    ImGui::Text(
        "%lu %s", results.size(), (results.size() == 1 ? "result" : "results"));

    // paginate results
    if (ImGui::ArrowButton("##prevPage", ImGuiDir_Left))
      currentPage = std::max(1, currentPage - 1);
    ImGui::SameLine();
    ImGui::Text("page");
    ImGui::SameLine();
    ImGui::SetNextItemWidth(20.f);
    if (ImGui::InputInt(paginateLabel.c_str(), &currentPage, 0))
      currentPage = std::min(std::max(currentPage, 1), numPages);
    ImGui::SameLine();
    if (ImGui::ArrowButton("##nextPage", ImGuiDir_Right))
      currentPage = std::min(numPages, currentPage + 1);
    ImGui::SameLine(0.0f, ImGui::GetFontSize() * 2.f);
    ImGui::SetNextItemWidth(5.f * ImGui::GetFontSize());
    if (ImGui::BeginCombo("results per page", numItemsOpt[numItemsInd])) {
      for (int i = 0; i < 4; i++) {
        const bool selected = (numItemsInd == i);
        if (ImGui::Selectable(numItemsOpt[i], selected)) {
          numItemsInd = i;
          numItemsPerPage = std::atoi(numItemsOpt[numItemsInd]);
          numPages = results.size() / numItemsPerPage;
          numPages += results.size() % numItemsPerPage == 0 ? 0 : 1;
          currentPage = std::min(currentPage, numPages);
          paginateLabel = "of " + std::to_string(numPages) + "##currentPage";
        }
        if (selected)
          ImGui::SetItemDefaultFocus();
      }
      ImGui::EndCombo();
    }
  }

  ImGui::BeginChild("Results",
      ImVec2(0, numItemsPerPage * ImGui::GetFontSize()),
      true,
      ImGuiWindowFlags_HorizontalScrollbar);
  static int selectedIndex;
  if (searched) {
    for (int i = (currentPage - 1) * numItemsPerPage;
         i < std::min((int)results.size(), currentPage * numItemsPerPage);
         i++) {
      const bool isSelected = (selectedIndex == i);
      if (results[i])
        if (ImGui::Selectable(results[i]->name().c_str(), isSelected)) {
          selectedIndex = i;
          selectedResult = results[selectedIndex]->name();
        }
      if (isSelected)
        ImGui::SetItemDefaultFocus();
    }
  } else {
    for (int i = 0; i < root.children().size(); i++) {
      auto &node = root.children().at_index(i);
      if (isOneOf(node.second->type(), displayTypes)) {
        const bool isSelected = (selectedIndex == i);
        if (ImGui::Selectable(node.first.c_str(), isSelected)) {
          selectedIndex = i;
          selectedResult = node.first;
        }
        if (isSelected)
          ImGui::SetItemDefaultFocus();
      }
    }
  }
  ImGui::EndChild();
}

bool SearchWidget::isOneOf(NT inNodeType, std::vector<NT> &nodeTypes)
{
  for (auto nt : nodeTypes)
    if (nt == inNodeType)
      return true;
  return false;
}

void SearchWidget::addCustomAction(std::string title,
    std::function<void(std::vector<ospray::sg::NodePtr> &)> searchOp,
    std::function<void()> displayOp,
    bool sameLine)
{
  if (sameLine)
    ImGui::SameLine();
  if (ImGui::Button(title.c_str())) {
    if (searched)
      searchOp(results);
    else
      displayOp();
  }
}
