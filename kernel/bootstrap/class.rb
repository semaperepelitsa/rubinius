# -*- encoding: us-ascii -*-

class Class
  def self.allocate
    Rubinius.primitive :class_s_allocate
    raise PrimitiveFailure, "Unable to create a new Class"
  end

  # An alias for subclasses to use that strictly allocates the memory
  # for an instance.
  alias_method :__allocate__, :allocate

  def set_superclass(sup)
    Rubinius.primitive :class_set_superclass
    raise TypeError, "superclass must be a Class (#{Rubinius::Type.object_class(sup)} given)"
  end

  private :set_superclass
end
