int ManagedLibrary_Component_get_id_Internal(const Component* component)
{
	return component->id;
}

int ManagedLibrary_Component_get_tag(MonoObject* this_ptr)
{
	Component* component;
	mono_field_get_value(this_ptr, native_handle_field, reinterpret_cast<void*>(&Component));
	return component->tag;
}
