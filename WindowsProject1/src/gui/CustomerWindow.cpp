// Made by Arjhine A. Ty.
#include "../../include/gui/CustomerWindow.h"
#include <wx/sizer.h>

CustomerWindow::CustomerWindow(const wxString& title, SystemManager* sysManager)
    : wxFrame(nullptr, wxID_ANY, title, wxDefaultPosition, wxSize(800, 600)), systemManager(sysManager) {
    
    CreateControls();
    BindEventHandlers();
    PopulateCustomerList();
}

void CustomerWindow::CreateControls() {
    panel = new wxPanel(this);

    wxBoxSizer* mainSizer = new wxBoxSizer(wxVERTICAL);
    
    // Customer List
    customerListCtrl = new wxListCtrl(panel, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxLC_REPORT | wxLC_SINGLE_SEL);
    customerListCtrl->AppendColumn("ID", wxLIST_FORMAT_LEFT, 50);
    customerListCtrl->AppendColumn("Name", wxLIST_FORMAT_LEFT, 200);
    customerListCtrl->AppendColumn("Phone", wxLIST_FORMAT_LEFT, 150);
    customerListCtrl->AppendColumn("Email", wxLIST_FORMAT_LEFT, 250);
    mainSizer->Add(customerListCtrl, 1, wxEXPAND | wxALL, 10);

    // Input Form
    wxFlexGridSizer* formSizer = new wxFlexGridSizer(2, 10, 10);
    formSizer->AddGrowableCol(1);

    nameTextCtrl = new wxTextCtrl(panel, wxID_ANY, "");
    phoneTextCtrl = new wxTextCtrl(panel, wxID_ANY, "");
    emailTextCtrl = new wxTextCtrl(panel, wxID_ANY, "");
    
    formSizer->Add(new wxStaticText(panel, wxID_ANY, "Name:"), 0, wxALIGN_CENTER_VERTICAL);
    formSizer->Add(nameTextCtrl, 1, wxEXPAND);
    formSizer->Add(new wxStaticText(panel, wxID_ANY, "Phone:"), 0, wxALIGN_CENTER_VERTICAL);
    formSizer->Add(phoneTextCtrl, 1, wxEXPAND);
    formSizer->Add(new wxStaticText(panel, wxID_ANY, "Email:"), 0, wxALIGN_CENTER_VERTICAL);
    formSizer->Add(emailTextCtrl, 1, wxEXPAND);

    mainSizer->Add(formSizer, 0, wxEXPAND | wxLEFT | wxRIGHT | wxBOTTOM, 10);

    // Action Buttons
    wxBoxSizer* buttonSizer = new wxBoxSizer(wxHORIZONTAL);
    addButton = new wxButton(panel, wxID_ANY, "Add");
    editButton = new wxButton(panel, wxID_ANY, "Edit");
    deleteButton = new wxButton(panel, wxID_ANY, "Delete");
    
    editButton->Disable();
    deleteButton->Disable();

    buttonSizer->Add(addButton, 0, wxALL, 5);
    buttonSizer->Add(editButton, 0, wxALL, 5);
    buttonSizer->Add(deleteButton, 0, wxALL, 5);

    mainSizer->Add(buttonSizer, 0, wxALIGN_CENTER | wxBOTTOM, 10);

    panel->SetSizer(mainSizer);
}

void CustomerWindow::BindEventHandlers() {
    addButton->Bind(wxEVT_BUTTON, &CustomerWindow::OnAdd, this);
    editButton->Bind(wxEVT_BUTTON, &CustomerWindow::OnEdit, this);
    deleteButton->Bind(wxEVT_BUTTON, &CustomerWindow::OnDelete, this);
    customerListCtrl->Bind(wxEVT_LIST_ITEM_SELECTED, &CustomerWindow::OnSelect, this);
}

void CustomerWindow::PopulateCustomerList() {
    customerListCtrl->DeleteAllItems();
    std::vector<Customer> customers = systemManager->getAllCustomers();
    for (const auto& customer : customers) {
        long index = customerListCtrl->InsertItem(customerListCtrl->GetItemCount(), wxString::Format(wxT("%i"), customer.id));
        customerListCtrl->SetItem(index, 1, customer.name);
        customerListCtrl->SetItem(index, 2, customer.phone);
        customerListCtrl->SetItem(index, 3, customer.email);
        customerListCtrl->SetItemData(index, customer.id);
    }
}

void CustomerWindow::OnAdd(wxCommandEvent& event) {
    wxString name = nameTextCtrl->GetValue();
    wxString phone = phoneTextCtrl->GetValue();
    wxString email = emailTextCtrl->GetValue();

    if (name.IsEmpty() || phone.IsEmpty() || email.IsEmpty()) {
        wxMessageBox("Please fill all fields.", "Error", wxOK | wxICON_ERROR);
        return;
    }

    Customer newCustomer;
    newCustomer.id = systemManager->getAllCustomers().empty() ? 1 : systemManager->getAllCustomers().back().id + 1;
    newCustomer.name = name.ToStdString();
    newCustomer.phone = phone.ToStdString();
    newCustomer.email = email.ToStdString();
    
    systemManager->addCustomer(newCustomer);
    PopulateCustomerList();

    nameTextCtrl->Clear();
    phoneTextCtrl->Clear();
    emailTextCtrl->Clear();
}

void CustomerWindow::OnEdit(wxCommandEvent& event) {
    if (selectedCustomerId == -1) return;

    Customer updatedCustomer;
    updatedCustomer.id = selectedCustomerId;
    updatedCustomer.name = nameTextCtrl->GetValue().ToStdString();
    updatedCustomer.phone = phoneTextCtrl->GetValue().ToStdString();
    updatedCustomer.email = emailTextCtrl->GetValue().ToStdString();

    systemManager->editCustomer(selectedCustomerId, updatedCustomer);
    PopulateCustomerList();
}

void CustomerWindow::OnDelete(wxCommandEvent& event) {
    if (selectedCustomerId == -1) return;

    systemManager->removeCustomer(selectedCustomerId);
    PopulateCustomerList();

    nameTextCtrl->Clear();
    phoneTextCtrl->Clear();
    emailTextCtrl->Clear();
    editButton->Disable();
    deleteButton->Disable();
    selectedCustomerId = -1;
}

void CustomerWindow::OnSelect(wxListEvent& event) {
    selectedCustomerId = event.GetItem().GetData();
    
    Customer* cust = systemManager->getCustomer(selectedCustomerId);
    if (cust) {
        nameTextCtrl->SetValue(cust->name);
        phoneTextCtrl->SetValue(cust->phone);
        emailTextCtrl->SetValue(cust->email);
        editButton->Enable();
        deleteButton->Enable();
    }
} 