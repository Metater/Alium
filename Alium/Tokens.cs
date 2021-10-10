using System;
using System.Collections.Generic;
using System.Text;

namespace Alium
{
    public abstract class Token
    {
        public string type;
    }

    public class SpecialToken : Token
    {
        public readonly string value;
        public SpecialToken(string value)
        {
            type = "special";
            this.value = value;
        }

        public override string ToString()
        {
            return $"SpecialToken(Value: \'{value}\')";
        }
    }

    public class ErrorToken : Token
    {
        public readonly string value;
        public ErrorToken(string value)
        {
            type = "error";
            this.value = value;
        }

        public override string ToString()
        {
            return value;
        }
    }

    public class MarkerToken : Token
    {
        public readonly char value;
        public MarkerToken(char value)
        {
            type = "marker";
            this.value = value;
        }

        public override string ToString()
        {
            return $"MarkerToken(Value: \'{value}\')";
        }
    }

    public class IntToken : Token
    {
        public readonly int value;
        public IntToken(int value)
        {
            type = "int";
            this.value = value;
        }

        public override string ToString()
        {
            return $"IntToken(Value: \'{value}\')";
        }
    }

    public class LongToken : Token
    {
        public readonly long value;
        public LongToken(long value)
        {
            type = "long";
            this.value = value;
        }

        public override string ToString()
        {
            return $"LongToken(Value: \'{value}\')";
        }
    }

    public class FloatToken : Token
    {
        public readonly float value;
        public FloatToken(float value)
        {
            type = "float";
            this.value = value;
        }

        public override string ToString()
        {
            return $"FloatToken(Value: \'{value}\')";
        }
    }

    public class DoubleToken : Token
    {
        public readonly double value;
        public DoubleToken(double value)
        {
            type = "double";
            this.value = value;
        }

        public override string ToString()
        {
            return $"DoubleToken(Value: \'{value}\')";
        }
    }

    public class IdentifierToken : Token
    {
        public readonly string value;
        public IdentifierToken(string value)
        {
            type = "identifier";
            this.value = value;
        }

        public override string ToString()
        {
            return $"IdentifierToken(Value: \'{value}\')";
        }
    }

    public class StringToken : Token
    {
        public readonly string value;
        public StringToken(string value)
        {
            type = "string";
            this.value = value;
        }

        public override string ToString()
        {
            return $"StringToken(Value: \'{value}\')";
        }
    }
}
