static void activate(GtkApplication* app, gpointer user_data) {
    GtkWidget* window;
    GtkWidget* grid, * button, * scrolled_window, * tree_view;
    GtkCellRenderer* renderer;
    GtkTreeViewColumn* column;
    GtkListStore* store;

    AppWidgets* widgets = g_malloc(sizeof(AppWidgets));

    window = gtk_application_window_new(app);
    gtk_window_set_title(GTK_WINDOW(window), u8"記帳程式");
    gtk_window_set_default_size(GTK_WINDOW(window), 600, 400);

    grid = gtk_grid_new();
    gtk_window_set_child(GTK_WINDOW(window), grid);

    widgets->entry_amount = gtk_entry_new();
    gtk_entry_set_placeholder_text(GTK_ENTRY(widgets->entry_amount), u8"金額");
    gtk_grid_attach(GTK_GRID(grid), widgets->entry_amount, 0, 0, 1, 1);

    widgets->entry_note = gtk_entry_new();
    gtk_entry_set_placeholder_text(GTK_ENTRY(widgets->entry_note), u8"備註");
    gtk_grid_attach(GTK_GRID(grid), widgets->entry_note, 1, 0, 1, 1);

    widgets->calendar = gtk_calendar_new();
    gtk_grid_attach(GTK_GRID(grid), widgets->calendar, 0, 1, 2, 1);

    GtkStringList* list = gtk_string_list_new((const char* const*)categories);
    widgets->combo_category = gtk_drop_down_new(G_LIST_MODEL(list), NULL);
    gtk_grid_attach(GTK_GRID(grid), widgets->combo_category, 2, 0, 1, 1);
    // 新增按鈕
    button = gtk_button_new_with_label(u8"新增記錄");
    gtk_grid_attach(GTK_GRID(grid), button, 3, 0, 1, 1);
    g_signal_connect(button, "clicked", G_CALLBACK(add_record), widgets);

    // 總計標籤
    widgets->tatol_w = gtk_label_new(u8"總金額：$0.00");
    gtk_grid_attach(GTK_GRID(grid), widgets->tatol_w, 0, 4, 4, 1);

    // 清單顯示區
    scrolled_window = gtk_scrolled_window_new();
    gtk_widget_set_vexpand(scrolled_window, TRUE);
    gtk_grid_attach(GTK_GRID(grid), scrolled_window, 0, 2, 4, 1);

    // 表格顯示記錄
    store = gtk_list_store_new(5, G_TYPE_STRING, G_TYPE_STRING, G_TYPE_STRING, G_TYPE_STRING, G_TYPE_FLOAT);
    widgets->list_store = GTK_WIDGET(store);

    tree_view = gtk_tree_view_new_with_model(GTK_TREE_MODEL(store));
    gtk_scrolled_window_set_child(GTK_SCROLLED_WINDOW(scrolled_window), tree_view);

    // 添加日期列
    renderer = gtk_cell_renderer_text_new();
    column = gtk_tree_view_column_new_with_attributes(u8"日期", renderer, "text", 0, NULL);
    gtk_tree_view_append_column(GTK_TREE_VIEW(tree_view), column);

    // 添加類別列
    column = gtk_tree_view_column_new_with_attributes(u8"類別", renderer, "text", 1, NULL);
    gtk_tree_view_append_column(GTK_TREE_VIEW(tree_view), column);

    // 添加金額列
    column = gtk_tree_view_column_new_with_attributes(u8"金額", renderer, "text", 2, NULL);
    gtk_tree_view_append_column(GTK_TREE_VIEW(tree_view), column);

    // 添加備註列
    column = gtk_tree_view_column_new_with_attributes(u8"備註", renderer, "text", 3, NULL);
    gtk_tree_view_append_column(GTK_TREE_VIEW(tree_view), column);

    // 添加總額列
    column = gtk_tree_view_column_new_with_attributes(u8"總計", renderer, "text", 4, NULL);
    gtk_tree_view_append_column(GTK_TREE_VIEW(tree_view), column);

    gtk_widget_show(window);
}