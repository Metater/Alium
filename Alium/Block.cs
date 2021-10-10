using System;
using System.Collections.Generic;
using System.Text;

namespace Alium
{
	public class Block
	{
		public List<string> identifiers = new List<string>();

		public bool HasAssignment => assignment != null;
		public ValueBlock assignment;

		public bool HasBody => children.Count != 0;
		public List<Block> children = new List<Block>();

		public bool HasNext => next != null;
		public Block next;
	}

	public class ValueBlock
	{
		public readonly ValueBlockType type;

		public Token literal;
		public string identifier;
		public FunctionBlock function;

		public ValueBlock(Token literal)
		{
			type = ValueBlockType.Literal;
			this.literal = literal;
		}

		public ValueBlock(string identifier)
		{
			type = ValueBlockType.Identifier;
			this.identifier = identifier;
		}

		public ValueBlock(FunctionBlock function)
		{
			type = ValueBlockType.Function;
			this.function = function;
		}

		public enum ValueBlockType
		{
			Literal,
			Identifier,
			Function
		}
	}

	public class FunctionBlock
	{
		public List<string> callChain = new List<string>();

		public List<ValueBlock> arguments = new List<ValueBlock>();
	}
}
