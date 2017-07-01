#include <mono/jit/jit.h>
#include <mono/metadata/assembly.h>
#include <mono/metadata/class.h>
#include <mono/metadata/debug-helpers.h>
#include <mono/metadata/mono-config.h>
#include <iostream>
#include "Test.h"
using namespace std;

Widget* widgets;
uint32_t num_widgets;
MonoClassField* native_handle_field;
MonoDomain* domain;
MonoClass* main_class;
MonoObject** managed_widgets;
//MonoDomain *domain;
int main()
{
	mono_set_assemblies_path("..\\lib\\mono\\4.5\\");
	
	const char* managed_binary_path = "./Main.dll";
	 domain = mono_jit_init(managed_binary_path);
	//获取应用域
	//domain = mono_jit_init(managed_binary_path);
	//mono运行时的配置
	//mono_set_dirs("/Library/Frameworks/Mono.framework/Home/lib", "/Library/Frameworks/Mono.framework/Home/etc");
	mono_config_parse(NULL);
	//加载程序集ManagedLibrary.dll
	MonoAssembly* assembly = mono_domain_assembly_open(domain, managed_binary_path);
	MonoImage* image = mono_assembly_get_image(assembly);


	mono_add_internal_call("ManagedLibrary.Widget::get_Number_Internal", reinterpret_cast<void*>(ManagedLibrary_Widget_get_Number_Internal));
	mono_add_internal_call("ManagedLibrary.Widget::get_Number2", reinterpret_cast<void*>(ManagedLibrary_Widget_get_Number2));
	mono_add_internal_call("ManagedLibrary.Widget::GetWidgets", reinterpret_cast<void*>(ManagedLibrary_Widget_GetWidgets));


	main_class = mono_class_from_name(image, "ManagedLibrary", "Widget");
	native_handle_field = mono_class_get_field_from_name(main_class, "native_handle");

	num_widgets = 5;
	widgets = new Widget[num_widgets];
	managed_widgets = new MonoObject*[num_widgets];

	for (uint32_t i = 0; i < num_widgets; ++i)
	{
		widgets[i].number = i * 4;

		MonoObject* managed_widget = mono_object_new(domain, main_class);
		mono_runtime_object_init(managed_widget);
		void* native_handle_value = &widgets[i];
		mono_field_set_value(managed_widget, native_handle_field, &native_handle_value);

		managed_widgets[i] = managed_widget;
	}



	//获取MonoClass
    main_class = mono_class_from_name(image, "ManagedLibrary", "Main");
	const bool include_namespace = true;
	//获取要调用的MonoMethodDesc
	MonoMethodDesc* entry_point_method_desc = mono_method_desc_new("ManagedLibrary.Main:EntryPoint()", include_namespace);
	MonoMethod* entry_point_method = mono_method_desc_search_in_class(entry_point_method_desc, main_class);

	mono_method_desc_free(entry_point_method_desc);
	//调用方法
	mono_runtime_invoke(entry_point_method, NULL, NULL, NULL);
	//释放应用域
	mono_jit_cleanup(domain);
	delete[] widgets;
	delete[] managed_widgets;
	system("pause");
	return 0;
}


int ManagedLibrary_Widget_get_Number_Internal(const Widget* widget)
{
	return widget->number;
}

int ManagedLibrary_Widget_get_Number2(MonoObject* this_ptr)
{
	Widget* widget;
	mono_field_get_value(this_ptr, native_handle_field, reinterpret_cast<void*>(&widget));
	return widget->number;
}

MonoArray* ManagedLibrary_Widget_GetWidgets()
{
	MonoArray* array = mono_array_new(domain, main_class, num_widgets);

	for (uint32_t i = 0; i < num_widgets; ++i)
	{
		mono_array_set(array, MonoObject*, i, managed_widgets[i]);
		

	}

	return array;
}