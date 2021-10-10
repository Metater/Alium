using System;
using System.Threading;
using System.Collections.Generic;

namespace Alium
{
	public class AliumParser
	{
		private readonly string source;
		private readonly AliumLexer lexer;

		public Block root;

		public Stack<int> pos = new Stack<int>();

		private Token token;

		public AliumParser(string source)
		{
			this.source = source;
			lexer = new AliumLexer(source);
			NextToken();
		}

		private void NextToken()
		{
			token = lexer.GetNextToken();
			if (token.type == "error") throw new Exception(token.ToString());
			Console.WriteLine(token);
		}

		private SpecialToken EatSpecial(string requiredValue = "")
		{
			if (token.type != "special") throw new Exception(lexer.GetErrorMessage($"expected special token, found {token}"));
			SpecialToken t = (SpecialToken)token;
			if (t.value != requiredValue && requiredValue != "") throw new Exception(lexer.GetErrorMessage($"expected special \'{requiredValue}\', found {token}"));
			NextToken();
			return t;
		}

		private MarkerToken EatMarker(char requiredValue = '\0')
		{
			if (token.type != "special") throw new Exception(lexer.GetErrorMessage($"expected marker token, found {token}"));
			MarkerToken t = (MarkerToken)token;
			if (t.value != requiredValue && requiredValue != '\0') throw new Exception(lexer.GetErrorMessage($"expected marker \'{requiredValue}\', found {token}"));
			NextToken();
			return t;
		}

		private IdentifierToken EatIdentifier(string requiredValue = "")
		{
			if (token.type != "identifier") throw new Exception(lexer.GetErrorMessage($"expected identifier token, found {token}"));
			IdentifierToken t = (IdentifierToken)token;
			if (t.value != requiredValue && requiredValue != "") throw new Exception(lexer.GetErrorMessage($"expected identifier \'{requiredValue}\', found {token}"));
			NextToken();
			return t;
		}

		public Block GetBlockTree()
		{
			root = new Block();
			while (!lexer.eof)
				AddBook();
			return root;
		}

		public void AddBook()
		{
			Block book = new Block();
			root.children.Add(book)
			book.identifiers.Add(EatIdentifier("bk").value);
			EatSpecial("whitespace");
			while (token.type == "identifier")
			{
				book.identifiers.Add(EatIdentifier().value);
				EatSpecial("whitespace");
			}
			EatMarker('{');
			AddPage();
		}

		public void AddPage()
		{
			Block book = new Block();
			root.children.Add(book);
		}

		public void AddAction()
		{

		}
	}
}