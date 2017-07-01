#pragma once
struct Widget
{
	int number;
};
void  DoSomething();
int ManagedLibrary_Widget_get_Number_Internal(const Widget* widget);
int ManagedLibrary_Widget_get_Number2(MonoObject* this_ptr);
MonoArray* ManagedLibrary_Widget_GetWidgets();